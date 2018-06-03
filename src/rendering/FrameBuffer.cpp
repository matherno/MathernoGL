//
// Created by matt on 26/05/18.
//

#include "mathernogl/rendering/FrameBuffer.h"
#include <mathernogl/utils/Logging.h>
#include <mathernogl/texture/TextureFactory.h>
#include <mathernogl/utils/GLUtils.h>
#include <mathernogl/Assert.h>

enum TextureTypes
  {
  textureColour = 0,
  textureDepth,
//  textureStencil,
  numTextureTypes,
  };

mathernogl::FrameBuffer::FrameBuffer()
  {
  textures.insert(textures.begin(), numTextureTypes, nullptr);
  }

bool checkFBOStatus()
  {
  const int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (fboStatus == GL_FRAMEBUFFER_UNSUPPORTED)
    mathernogl::logError("Frame Buffer initialisation failed. Not supported on this GPU. ");
  else if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
    mathernogl::logError("Frame Buffer initialisation failed. Incomplete missing attachment. ");
  else if (fboStatus == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
    mathernogl::logError("Frame Buffer initialisation failed. Incomplete multisampler. ");
  else if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    mathernogl::logError("Frame Buffer initialisation failed. Status code: " + std::to_string(fboStatus));
  else
    return true;
  return false;
  }

bool mathernogl::FrameBuffer::initialise(uint width, uint height, bool needAlphaChannel, bool useDepthTexture, bool gammaCorrect)
  {
  fboWidth = width;
  fboHeight = height;
  clearGLErrors();
  glGenFramebuffers(1, &fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, fboID);
  ASSERT_NO_GL_ERROR();

  clearGLErrors();
  if (useDepthTexture)
    {
    textures[textureDepth].reset(createDepthTexture(width, height));
    if (textures[textureDepth])
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[textureDepth]->glTexType, textures[textureDepth]->glTexID, 0);
    }
  else
    {
    GLuint depthAttachment;
    glGenRenderbuffers(1, &depthAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER, depthAttachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
  ASSERT_NO_GL_ERROR();

  clearGLErrors();
  TextureOptions textureOptions;
  textureOptions.gammaCorrect = gammaCorrect;
  textureOptions.genMipMaps = false;
  textureOptions.wrapping = REPEAT;
  textureOptions.filtering = NEAREST;
  textures[textureColour].reset(createEmptyTexture(width, height, needAlphaChannel ? 4 : 3, textureOptions));
  ASSERT_NO_GL_ERROR();

  clearGLErrors();
  if (textures[textureColour])
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[textureColour]->glTexType, textures[textureColour]->glTexID, 0);
  ASSERT_NO_GL_ERROR();

  bool success = false;
  if (textures[textureColour] && (!useDepthTexture || textures[textureDepth]))
    success = checkFBOStatus();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return success;
  }

bool mathernogl::FrameBuffer::initialiseMultisampled(uint width, uint height, uint numSamples, bool gammaCorrect)
  {
  fboWidth = width;
  fboHeight = height;
  clearGLErrors();
  glGenFramebuffers(1, &fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, fboID);
  ASSERT_NO_GL_ERROR();

  clearGLErrors();
  GLuint depthAttachment;
  glGenRenderbuffers(1, &depthAttachment);
  glBindRenderbuffer(GL_RENDERBUFFER, depthAttachment);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, GL_DEPTH_COMPONENT24, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthAttachment);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  ASSERT_NO_GL_ERROR();

  clearGLErrors();
  bool success = false;
  textures[textureColour].reset(createMultiSampleTexture(width, height, numSamples, gammaCorrect));
  if (textures[textureColour])
    {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[textureColour]->glTexType, textures[textureColour]->glTexID, 0);
    success = checkFBOStatus();
    }
  ASSERT_NO_GL_ERROR();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return success;
  }

void mathernogl::FrameBuffer::cleanUp()
  {
  glDeleteFramebuffers(1, &fboID);
  }

const std::shared_ptr<mathernogl::Texture> mathernogl::FrameBuffer::getColourTexture() const
  {
  return textures[textureColour];
  }

const std::shared_ptr<mathernogl::Texture> mathernogl::FrameBuffer::getDepthTexture() const
  {
  return textures[textureDepth];
  }

void mathernogl::FrameBuffer::clear()
  {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

void mathernogl::FrameBuffer::blitToScreen(uint screenWidth, uint screenHeight)
  {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID);
  glDrawBuffer(GL_BACK);
  glBlitFramebuffer(0, 0, fboWidth, fboHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  }

void mathernogl::FrameBuffer::blitToFBO(mathernogl::FrameBuffer* fbo, bool includeDepth, bool includeStencil)
  {
  GLbitfield buffersToBlit = GL_COLOR_BUFFER_BIT;
  if (includeDepth)
    buffersToBlit |= GL_DEPTH_BUFFER_BIT;
  if (includeStencil)
    buffersToBlit |= GL_STENCIL_BUFFER_BIT;

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->getGLID());
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID);
  glDrawBuffer(GL_BACK);
  glBlitFramebuffer(0, 0, fboWidth, fboHeight, 0, 0, fbo->getWidth(), fbo->getHeight(), buffersToBlit, GL_NEAREST);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  }

