//
// Created by matt on 24/12/17.
//

#include "RenderableParticles.h"

using namespace mathernogl;

RenderableParticles::RenderableParticles(uint id, bool renderAsPoints) : Renderable(id, DRAW_STAGE_TRANSPARENT), renderAsPoints(renderAsPoints)
  {}

void RenderableParticles::initialise(RenderContext* renderContext)
  {
  particlesBuffer.init();
  vao.init();
  vao.bind();

  if (renderAsPoints)
    {
    vao.linkBufferAsFloats(particlesBuffer, 0,                 sizeof(float), 3, 0, false);       // positions
    vao.linkBufferAsFloats(particlesBuffer, sizeof(float) * 3, sizeof(float) * 3, 1, 1, false);   // fade values
    std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/ParticlesVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/ParticlesFS.glsl") };
    shaderProgram = renderContext->getSharedShaderProgram(&shaders);
    }
  else
    {
    mathernogl::GPUBufferStatic vertexBuffer;
    vertexBuffer.init();
    vertexBuffer.copyDataFloat({-0.5f,-0.5f, 0.5f,-0.5f, -0.5f,0.5f, -0.5f,0.5f, 0.5f,-0.5f, 0.5f,0.5f});
    vao.linkBufferAsFloats(vertexBuffer, 2, 0, false);                                       // vertices    (shared between particles)
    vao.linkBufferAsFloats(particlesBuffer, 0,               sizeof(float)*2, 3, 1, true);   // positions   (per particle)
    vao.linkBufferAsFloats(particlesBuffer, sizeof(float)*3, sizeof(float)*4, 1, 2, true);   // fade values (per particle)
    vao.linkBufferAsFloats(particlesBuffer, sizeof(float)*4, sizeof(float)*4, 1, 3, true);   // texture index (per particle)
    vao.unbind();
    vertexBuffer.cleanUp();
    std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/ParticleQuadsVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/ParticleQuadsFS.glsl") };
    shaderProgram = renderContext->getSharedShaderProgram(&shaders);
    }

  glEnable(GL_PROGRAM_POINT_SIZE);
  }

void RenderableParticles::cleanUp(RenderContext* renderContext)
  {
  }

void RenderableParticles::render(RenderContext* renderContext)
  {
  mathernogl::clearGLErrors();
  renderContext->activateShaderProgram(shaderProgram);
  shaderProgram->setVarFloat("inSize", size);
  shaderProgram->setVarVec3("inColour", colour);
  vao.bind();
  setFaceCulling(false);
  setAlphaBlending(true, additiveBlending);
  setDepthTest(depthTesting);
  if (renderAsPoints)
    renderPoints(renderContext);
  else
    renderQuads(renderContext);
  ASSERT_NO_GL_ERROR();
  }

void RenderableParticles::renderPoints(RenderContext* renderContext)
  {
  int particleCount = 0;
  std::vector<float> posData;
  for(auto iter = emitter->begin(); iter != emitter->end(); iter++)
    {
    ++particleCount;
    Particle* particle = iter->get();
    Vector3D pos = particle->position;
    posData.push_back(pos.x);
    posData.push_back(pos.y);
    posData.push_back(pos.z);
    posData.push_back(calcFadeValue(particle));
    }
  particlesBuffer.copyDataFloat(posData);
  glDrawArrays(GL_POINTS, 0, particleCount);
  }

void RenderableParticles::renderQuads(RenderContext* renderContext)
  {
  if (!textures.empty())
    {
    shaderProgram->setVarInt("inNumTexMapCellsX", numTexAtlasColumns);
    shaderProgram->setVarInt("inNumTexMapCellsY", numTexAtlasRows);
    shaderProgram->setVarFloat("inColourMixFactor", colourMixFactor);
    for (int index = 0; index < textures.size(); ++index)
      shaderProgram->setVarIntArray("inTexSamplers", index, renderContext->bindTexture(textures[index]));
    }
  else
    shaderProgram->setVarFloat("inColourMixFactor", 1);

  int particleCount = 0;
  std::vector<float> posData;
  for(auto iter = emitter->begin(); iter != emitter->end(); iter++)
    {
    ++particleCount;
    Particle* particle = iter->get();
    Vector3D pos = particle->position;
    posData.push_back(pos.x);
    posData.push_back(pos.y);
    posData.push_back(pos.z);
    posData.push_back(calcFadeValue(particle));
    posData.push_back(clampi(particle->texIndex, 0, (int)textures.size() - 1));
    }
  particlesBuffer.copyDataFloat(posData);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particleCount);
  }

void RenderableParticles::setEmitter(ParticleEmitterPtr emitter)
  {
  this->emitter = emitter;
  }

void RenderableParticles::setColour(const Vector3D& colour)
  {
  this->colour = colour;
  }

void RenderableParticles::setParticleSize(double size)
  {
  this->size = size;
  }

void RenderableParticles::addTextureAtlas(TexturePtr textureAtlas)
  {
  textures.push_back(textureAtlas);
  }

float RenderableParticles::calcFadeValue(Particle* particle) const
  {
  return 1.0f - (float) particle->timeToLive / emitter->getTimeAlive();
  }


