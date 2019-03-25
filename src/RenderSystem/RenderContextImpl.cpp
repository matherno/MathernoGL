//
// Created by matherno on 17/11/17.
//

#include "RenderContextImpl.h"
#include "RenderableSetImpl.h"

using namespace mathernogl;

bool RenderContextImpl::initialise(const RenderInitConfig* initConfig)
  {
  nextTexBoundLocal = 1;
  nextMeshStorageID = 1;
  nextRenderableID = 1;

  clearGLErrors();
  window.reset(initGL(initConfig->windowName, initConfig->windowWidth, initConfig->windowHeight, initConfig->fullscreen, false, true));
  if (window)
    {
    window->setClearColour(0, 0, 0);
    renderableSet.reset(new RenderableSetImpl(getNextRenderableID()));
    registerStandardDrawStages();
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxNumTextureLocations);
    if(!initialiseFrameBuffers(initConfig))
      return false;
    ASSERT_NO_GL_ERROR();
    return true;
    }
  return false;
  }

bool RenderContextImpl::cleanUp()
  {
  reset();
  screenFBO_A->cleanUp();
  screenFBO_A.reset();
  screenFBO_B->cleanUp();
  screenFBO_B.reset();
  multiSampledFBO->cleanUp();
  multiSampledFBO.reset();
  window->close();
  return true;
  }

void RenderContextImpl::reset()
  {
  resourceCache.forEachShaderProgram([](ShaderProgramPtr shaderProgram)
    {
    shaderProgram->cleanUp();
    });
  resourceCache.forEachMeshStorage([](MeshStoragePtr meshStorage)
    {
    meshStorage->cleanUp();
    });
  resourceCache.forEachTexture([](TexturePtr texture)
    {
    texture->cleanUp();
    });
  resourceCache.forEachVoxelStorage([](VoxelStoragePtr voxelStorage)
    {
    voxelStorage->cleanUp();
    });
  resourceCache.clearAll();
  texIDsToBoundLocals.clear();
  fboStack.clear();
  drawStages.clear();
  registerStandardDrawStages();
  postProcessingSteps.clear();
  voxelBatchManager.cleanUp(this);
  }

uint RenderContextImpl::getNextRenderableID()
  {
  return nextRenderableID++;
  }

void RenderContextImpl::setWorldToCamera(const Matrix4& transform)
  {
  worldToCameraTransform = transform;
  worldToClipTransform = worldToCameraTransform * cameraToClipTransform;

  viewDirection = Vector3D(0, 0, 1) * matrixInverse(worldToCameraTransform);
  viewDirection.makeUniform();
  }

void RenderContextImpl::setCameraToClip(const Matrix4& transform)
  {
  cameraToClipTransform = transform;
  worldToClipTransform = worldToCameraTransform * cameraToClipTransform;

  Matrix4 clipToCamera = mathernogl::matrixInverse(cameraToClipTransform);
  viewZNearPlane = (Vector3D(0, 0, -1) * clipToCamera).z;
  viewZFarPlane = (Vector3D(0, 0, 1) * clipToCamera).z;
  }

void RenderContextImpl::render()
  {
  if (isRendering)
    return;
  isRendering = true;
  boundShaderID = 0;

  renderShadowMap();

  //  activate multi sampled FBO
  pushFrameBuffer(multiSampledFBO);
  multiSampledFBO->clear();

  //  render each stage
  for (int stage : drawStages)
    {
    if (stage >= DRAW_STAGE_OVERLAY)
      glClear(GL_DEPTH_BUFFER_BIT);
    renderDrawStage(stage, false);
    }
  activeDrawStage = DRAW_STAGE_NONE;

  //  blit multi sampled fbo to screen
  popFrameBuffer();
  window->clear();
  multiSampledFBO->blitToScreen(window->getWidth(), window->getHeight());

  window->update();
  isRendering = false;
  }

void RenderContextImpl::renderDrawStage(int drawStage, bool shadowMap)
  {
  activeDrawStage = drawStage;
  transformStack.clear();
  pushTransform(renderableSet->getTransform());
  setClippingPlane(*renderableSet->getClippingPlane());
  if (shadowMap)
    renderableSet->renderShadowMap(this);
  else
    renderableSet->render(this);
  disableClippingPlane();
  popTransform();

  if (!shadowMap && postProcessingSteps.count(drawStage) > 0)
    performPostProcessing(postProcessingSteps[drawStage]);
  }

void RenderContextImpl::performPostProcessing(std::list<PostProcStepHandlerPtr>& ppSteps)
  {
  popFrameBuffer();

  // do post processing
  multiSampledFBO->blitToFBO(screenFBO_A.get(), true, false);
  bool fboAActive = true;
  for (PostProcStepHandlerPtr postProc : ppSteps)
    {
    FrameBufferPtr activeFBO = fboAActive ? screenFBO_B : screenFBO_A;
    FrameBufferPtr inactiveFBO = fboAActive ? screenFBO_A : screenFBO_B;
    fboAActive = !fboAActive;

    pushFrameBuffer(activeFBO);
    getTopFrameBuffer()->clear();
    postProc->render(this, inactiveFBO);
    popFrameBuffer();
    }

  // blit most recent frame buffer back to multi sampled fbo
  clearFrameBufferActiveStack();
  window->clear();
  if(fboAActive)
    screenFBO_A->blitToFBO(multiSampledFBO.get(), false, false);
  else
    screenFBO_B->blitToFBO(multiSampledFBO.get(), false, false);

  pushFrameBuffer(multiSampledFBO);
  }

bool RenderContextImpl::isWindowOpen() const
  {
  return window->isOpen();
  }

void RenderContextImpl::activateShaderProgram(ShaderProgramPtr shaderProgram)
  {
  if (shaderProgram->getGLID() != boundShaderID)
    {
    shaderProgram->enable();
    boundShaderID = shaderProgram->getGLID();
    }

  //todo: keep state of whether or not each shader program has already been given these variables that frame (not for clipping uniforms)

  //  transform matrices
  if (renderingShadowMap)
    {
    shaderProgram->setVarMat4(SHADER_VAR_WORLD_TO_CAMERA, shadowMapWorldToCamera, true);
    shaderProgram->setVarMat4(SHADER_VAR_CAMERA_TO_CLIP, shadowMapCameraToClip, true);
    }
  else
    {
    shaderProgram->setVarMat4(SHADER_VAR_WORLD_TO_CAMERA, worldToCameraTransform, true);
    shaderProgram->setVarMat4(SHADER_VAR_CAMERA_TO_CLIP, cameraToClipTransform, true);
    }
  if (getStackedTransform())
    shaderProgram->setVarMat4(SHADER_VAR_VERT_TO_WORLD, *getStackedTransform()->getTransformMatrix(), true);
  else
    shaderProgram->setVarMat4(SHADER_VAR_VERT_TO_WORLD, Matrix4(1), true);

  //  viewing stuff
  shaderProgram->setVarVec3(SHADER_VAR_VIEW_DIR, viewDirection, true);
  shaderProgram->setVarFloat(SHADER_VAR_VIEW_NEAR, (float) viewZNearPlane, true);
  shaderProgram->setVarFloat(SHADER_VAR_VIEW_FAR, (float) viewZFarPlane, true);

  //  clipping
  bool enableClipping = clipPlane.x != 0 || clipPlane.y != 0 || clipPlane.z != 0;
  shaderProgram->setVarInt(SHADER_VAR_CLIPPING_ENABLED, enableClipping ? 1 : 0, true);
  if (enableClipping)
    shaderProgram->setVarVec4(SHADER_VAR_CLIP_PLANE, clipPlane, true);

  //  misc
  shaderProgram->setVarVec2(SHADER_VAR_SCREEN_SIZE, getWindow()->getSize(), true);
  shaderProgram->setVarInt(SHADER_VAR_TIME_MS, (int)(getTimeMS() - startTime), true);
  shaderProgram->setVarVec3(SHADER_VAR_LIGHT_DIR, lightDirection, true);
  if (!renderingShadowMap && shadowMapEnabled)
    {
    shaderProgram->setVarInt(SHADER_VAR_SHADOW_MAP, bindTexture(shadowMapFBO->getDepthTexture()), true);
    shaderProgram->setVarInt(SHADER_VAR_USE_SHADOW_MAP, 1, true);
    shaderProgram->setVarMat4(SHADER_VAR_SHADOW_MAP_PROJ, shadowMapProjection, true);
    }
  else
    shaderProgram->setVarInt(SHADER_VAR_USE_SHADOW_MAP, 0, true);
  }

void logCreateShaderProgram(ShaderProgramPtr shaderProgram, const std::vector<mathernogl::Shader>* shaders)
  {
  string msg = "Created shader program: " + std::to_string(shaderProgram->getGLID());
  for (const mathernogl::Shader& shader : *shaders)
    msg += ", '" + shader.filePath + "'";
  logInfo(msg);
  }

ShaderProgramPtr RenderContextImpl::getSharedShaderProgram(const std::vector<mathernogl::Shader>* shaders)
  {
  if (ShaderProgramPtr cachedShaderProgram = resourceCache.getShaderProgram(shaders))
    return cachedShaderProgram;

  ShaderProgramPtr shaderProgram (new ShaderProgram());
  shaderProgram->init(*shaders);
  resourceCache.addShaderProgram(shaderProgram, shaders);
  logCreateShaderProgram(shaderProgram, shaders);
  return shaderProgram;
  }
MeshStoragePtr RenderContextImpl::getSharedMeshStorage(const std::string& objFilePath)
  {
  if (MeshStoragePtr cachedMeshStorage = resourceCache.getMeshStorage(objFilePath))
    return cachedMeshStorage;

  MeshStoragePtr meshStorage(new MeshStorage(nextMeshStorageID++));
  loadObj(objFilePath, &meshStorage->indices, &meshStorage->vertices, &meshStorage->normals, &meshStorage->texCoords, &meshStorage->colours, false);
  meshStorage->setColoursPerFace(true);
  meshStorage->invalidateMinMax();
  if(meshStorage->initialiseVAO())
    {
    resourceCache.addMeshStorage(meshStorage, objFilePath);
    logInfo("Created mesh storage: " + std::to_string(meshStorage->getID()) + ", '" + objFilePath + "'");
    return meshStorage;
    }
  else
    {
    logWarning("Failed to create mesh storage!: " + std::to_string(meshStorage->getID()) + ", '" + objFilePath + "'");
    return nullptr;
    }
  }

MeshStoragePtr RenderContextImpl::createEmptyMeshStorage()
  {
  return std::make_shared<MeshStorage>(nextMeshStorageID++);
  }

MeshStorageInstancedPtr RenderContextImpl::createInstancedMeshStorage(const string& objFilePath, uint maxNumInstances)
  {
  MeshStorageInstancedPtr meshStorage(new MeshStorageInstanced(nextMeshStorageID++, maxNumInstances));
  loadObj(objFilePath, &meshStorage->indices, &meshStorage->vertices, &meshStorage->normals, &meshStorage->texCoords);
  meshStorage->invalidateMinMax();
  if(!meshStorage->initialiseVAO())
    {
    logWarning("Failed to create mesh storage!: " + std::to_string(meshStorage->getID()) + ", '" + objFilePath + "'");
    return nullptr;
    }
  return meshStorage;
  }


VoxelStoragePtr RenderContextImpl::getSharedVoxelStorage(const string& mgvFilePath)
  {
  if (VoxelStoragePtr cachedVoxelStorage = resourceCache.getVoxelStorage(mgvFilePath))
    return cachedVoxelStorage;

  VoxelStoragePtr voxelStorage(new VoxelStorage(nextVoxelStorageID++));
  loadVoxelMGVFile(mgvFilePath, &voxelStorage->colours, &voxelStorage->voxels, true);
  voxelStorage->invalidateMinMax();

  if(voxelStorage->initialiseVAO())
    {
    resourceCache.addVoxelStorage(voxelStorage, mgvFilePath);
    logInfo("Created voxel storage: " + std::to_string(voxelStorage->getID()) + ", '" + mgvFilePath + "'");
    return voxelStorage;
    }
  else
    {
    logWarning("Failed to create voxel storage!: " + std::to_string(voxelStorage->getID()) + ", '" + mgvFilePath + "'");
    return nullptr;
    }
  }

void RenderContextImpl::pushTransform(const mathernogl::Transform* transform)
  {
  transformStack.push(transform);
  }

void RenderContextImpl::popTransform()
  {
  transformStack.pop();
  }

const Transform* RenderContextImpl::getStackedTransform()
  {
  return transformStack.getTop();
  }

Window* RenderContextImpl::getWindow()
  {
  return window.get();
  }

const Window* RenderContextImpl::getWindow() const
  {
  return window.get();
  }

const Matrix4* RenderContextImpl::getWorldToCamera() const
  {
  return &worldToCameraTransform;
  }

const Matrix4* RenderContextImpl::getCameraToClip() const
  {
  return &cameraToClipTransform;
  }

const Matrix4* RenderContextImpl::getWorldToClip() const
  {
  return &worldToClipTransform;
  }

TexturePtr RenderContextImpl::getSharedTexture(const string& imageFilePath, TextureOptions options)
  {
  if (TexturePtr texture = resourceCache.getTexture(imageFilePath))
    return texture;

  TexturePtr texture = TexturePtr(createTextureFromFile(imageFilePath, options));
  resourceCache.addTexture(texture, imageFilePath);
  logInfo("Created texture: " + std::to_string(texture->glTexID) + ", '" + imageFilePath + "'");
  return texture;
  }

TexturePtr RenderContextImpl::createEmptyTexture(uint width, uint height, uint bytesPerPixel, TextureOptions options)
  {
  return TexturePtr(mathernogl::createEmptyTexture(width, height, bytesPerPixel, options));
  }

FontPtr RenderContextImpl::getSharedFont(const string& fntFilePath, const string& glyphsFilePath, float sizeScaling)
  {
  TexturePtr glyphsTexture = getSharedTexture(glyphsFilePath);
  FontDefinitionPtr fontDefinition = resourceCache.getFontDefinition(fntFilePath);
  if (!fontDefinition)
    {
    fontDefinition = readFontFile(fntFilePath);
    resourceCache.addFontDefinition(fontDefinition, fntFilePath);
    logInfo("Created font: '" + fntFilePath + "'");
    }
  return FontPtr(new Font(fontDefinition, glyphsTexture, sizeScaling));
  }


uint RenderContextImpl::bindTexture(TexturePtr texture)
  {
  uint glBindLocation = 0;
  if (texIDsToBoundLocals.find(texture->glTexID) != texIDsToBoundLocals.end())
    {
    glBindLocation = texIDsToBoundLocals[texture->glTexID];
    }
  else
    {
    if (nextTexBoundLocal >= maxNumTextureLocations)
      {
      logWarning("Ran out of texture bound locations! Max: " + std::to_string(maxNumTextureLocations));
      nextTexBoundLocal = 1;
      }

    //  nextTexBoundLocal is initialised to 1, and active texture reset back to 0 after binding,
    //      then external texture bindings don't mess things up
    glBindLocation = nextTexBoundLocal++;
    glActiveTexture(GL_TEXTURE0 + glBindLocation);
    glBindTexture(texture->glTexType, texture->glTexID);
    texIDsToBoundLocals[texture->glTexID] = glBindLocation;
    glActiveTexture(GL_TEXTURE0);
    }
  return glBindLocation;
  }

RenderableSetPtr RenderContextImpl::createRenderableSet()
  {
  return RenderableSetPtr(new RenderableSetImpl(getNextRenderableID()));
  }

void RenderContextImpl::setClippingPlane(const Vector4D& plane)
  {
  bool clippingEnabled = isClippingEnabled();
  if (plane.x != 0 || plane.y != 0 || plane.z != 0)
    {
    if (!clippingEnabled)
      glEnable(GL_CLIP_DISTANCE0);
    clipPlane = plane;
    }
  }

void RenderContextImpl::disableClippingPlane()
  {
  if (isClippingEnabled())
    {
    glDisable(GL_CLIP_DISTANCE0);
    clipPlane.set(0, 0, 0, 0);
    }
  }

bool RenderContextImpl::isClippingEnabled()
  {
  return (clipPlane.x != 0 || clipPlane.y != 0 || clipPlane.z != 0);
  }

bool RenderContextImpl::registerDrawStage(int drawStage)
  {
  if (drawStages.count(drawStage) == 0)
    {
    drawStages.insert(drawStage);
    return true;
    }
  return false;
  }

void RenderContextImpl::registerStandardDrawStages()
  {
  registerDrawStage(DRAW_STAGE_OPAQUE);
  registerDrawStage(DRAW_STAGE_TRANSPARENT);
  registerDrawStage(DRAW_STAGE_OVERLAY);
  registerDrawStage(DRAW_STAGE_UI);
  }

FrameBufferPtr RenderContextImpl::getTopFrameBuffer()
  {
  if (fboStack.empty())
    return nullptr;
  return *fboStack.begin();
  }

void RenderContextImpl::pushFrameBuffer(FrameBufferPtr fbo)
  {
  fboStack.push_front(fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->getGLID());
  }

void RenderContextImpl::popFrameBuffer()
  {
  fboStack.pop_front();
  if (fboStack.empty())
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  else
    glBindFramebuffer(GL_FRAMEBUFFER, getTopFrameBuffer()->getGLID());
  }

void RenderContextImpl::addPostProcessingStep(PostProcStepHandlerPtr handler)
  {
  const int drawStage = handler->getDrawStage();
  if (postProcessingSteps.count(drawStage) == 0)
    postProcessingSteps[drawStage] = std::list<PostProcStepHandlerPtr>();
  postProcessingSteps[drawStage].push_back(handler);
  }

void RenderContextImpl::removePostProcessingStep(uint id)
  {
  for (auto& pair : postProcessingSteps)
    {
    pair.second.remove_if(
    [id](const PostProcStepHandlerPtr& handler)
      {
      return handler->getID() == id;
      }
    );
    }
  }

uint RenderContextImpl::getNextPostProcessingStepID()
  {
  return nextPostProcStepID++;
  }

bool RenderContextImpl::initialiseFrameBuffers(const RenderInitConfig* initConfig)
  {
  screenFBO_A = createFrameBuffer();
  screenFBO_B = createFrameBuffer();
  if (!screenFBO_A || !screenFBO_B)
    return false;

  multiSampledFBO.reset(new FrameBuffer());
  if (initConfig->antiAliasing)
    {
    if (!multiSampledFBO->initialiseMultisampled(initConfig->windowWidth, initConfig->windowHeight, 4, true))
      return false;
    }
  else
    {
    multiSampledFBO = createFrameBuffer();
    if (!multiSampledFBO)
      return false;
    }

  return true;
  }

FrameBufferPtr RenderContextImpl::createFrameBuffer()
  {
  FrameBufferPtr fbo(new FrameBuffer());
  fbo->initialise(getWindow()->getWidth(), getWindow()->getHeight(), false, true, true);
  return fbo;
  }

void RenderContextImpl::clearFrameBufferActiveStack()
  {
  while(getTopFrameBuffer())
    popFrameBuffer();
  }

void RenderContextImpl::renderShadowMap()
  {
  if (!shadowMapEnabled || shadowMapValid)
    return;

  renderingShadowMap = true;
  pushFrameBuffer(shadowMapFBO);
  shadowMapFBO->clear();
  glPolygonOffset(1.5, -5);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, shadowMapWidth, shadowMapHeight);
  for (int stage : drawStages)
    {
    if (stage <= shadowMapDrawStage)
      renderDrawStage(stage, true);
    }
  popFrameBuffer();
  glViewport(0, 0, window->getWidth(), window->getHeight());
  glDisable(GL_POLYGON_OFFSET_FILL);
  renderingShadowMap = false;
  shadowMapValid = true;
  }

void RenderContextImpl::configureShadowMap(bool enable, Vector3D position, Vector3D direction, double fov, double zDistance, uint width, uint height)
  {
  shadowMapEnabled = enable;
  if (!shadowMapEnabled)
    return;

  if (direction.x == 0 && direction.z == 0)
    direction.x = 0.1;
  direction.makeUniform();

  double yaw = ccwAngleBetween(Vector2D(0, -1), Vector2D(direction.x, direction.z).getUniform());
  Matrix4 yawRotation = matrixYaw(yaw);
  double pitch = 0;
  if (direction.y != 0)
    {
    Vector3D yawedZAxis = Vector3D(0, 0, -1) * yawRotation;
    pitch = angleBetween(yawedZAxis, direction);
    if (direction.y < 0)
      pitch *= -1;
    }

  shadowMapWorldToCamera = matrixTranslate(position * -1) * matrixYaw(-yaw) * matrixPitch(-pitch);
  shadowMapCameraToClip = matrixOrthogonal(fov, (double)width / (double)height, -1 * zDistance);
  shadowMapProjection = shadowMapWorldToCamera * shadowMapCameraToClip * matrixScale(0.5) * matrixTranslate(0.5, 0.5, 0.5);
  lightDirection = direction;

  if (shadowMapWidth != width || shadowMapHeight != height)
    {
    shadowMapWidth = width;
    shadowMapHeight = height;
    setupShadowMapFBO();
    }
  invalidateShadowMap();
  }

void RenderContextImpl::invalidateShadowMap()
  {
  shadowMapValid = false;
  }

void RenderContextImpl::setupShadowMapFBO()
  {
  if (shadowMapFBO)
    shadowMapFBO->cleanUp();
  shadowMapFBO.reset(new FrameBuffer());
  shadowMapFBO->initialiseForShadowMapping(shadowMapWidth, shadowMapHeight);
  }

void RenderContextImpl::setShadowMapDrawStage(int drawStage)
  {
  shadowMapDrawStage = drawStage;
  }





