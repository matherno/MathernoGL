//
// Created by matt on 26/05/18.
//

#include "mathernogl/rendering/FrameBuffer.h"
#include <mathernogl/utils/Logging.h>
#include <mathernogl/texture/TextureFactory.h>
#include <mathernogl/utils/GLUtils.h>
#include <mathernogl/Assert.h>

#define MAX_COL_OUTPUTS 8

enum TextureTypes
  {
  textureDepth = 0,
  textureStencil,
  textureColour0,
  textureColour1,
  textureColour2,
  textureColour3,
  textureColour4,
  textureColour5,
  textureColour6,
  textureColour7,
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

bool mathernogl::FrameBuffer::initialise(uint width, uint height, bool needAlphaChannel, bool useDepthTexture, bool gammaCorrect, int numColourOutputs)
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

  std::vector<GLenum> colourAttachments;
  for (int colAttachment = 0; colAttachment < std::min(numColourOutputs, MAX_COL_OUTPUTS); ++colAttachment)
    {
    const int textureID = textureColour0 + colAttachment;
    clearGLErrors();
    TextureOptions textureOptions;
    textureOptions.gammaCorrect = gammaCorrect;
    textureOptions.genMipMaps = false;
    textureOptions.wrapping = REPEAT;
    textureOptions.filtering = NEAREST;
    textures[textureID].reset(createEmptyTexture(width, height, needAlphaChannel ? 4 : 3, textureOptions));
    ASSERT_NO_GL_ERROR();

    clearGLErrors();
    if (textures[textureID])
      {
      GLenum attachment = (GLenum) (GL_COLOR_ATTACHMENT0 + colAttachment);
      colourAttachments.push_back(attachment);
      glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textures[textureID]->glTexType, textures[textureID]->glTexID, 0);
      }
    ASSERT_NO_GL_ERROR();
    }

  bool success = false;
  if ((numColourOutputs > 0 && textures[textureColour0]) && (!useDepthTexture || textures[textureDepth]))
    {
    glDrawBuffers((GLsizei) colourAttachments.size(), colourAttachments.data());
    success = checkFBOStatus();
    }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return success;
  }

bool mathernogl::FrameBuffer::initialiseMultisampled(uint width, uint height, uint numSamples, bool gammaCorrect, int numColourOutputs)
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

  bool success = true;
  std::vector<GLenum> colourAttachments;
  for (int colAttachment = 0; colAttachment < std::min(numColourOutputs, MAX_COL_OUTPUTS); ++colAttachment)
    {
    const int textureID = textureColour0 + colAttachment;
    clearGLErrors();
    textures[textureID].reset(createMultiSampleTexture(width, height, numSamples, gammaCorrect));
    GLenum attachment = (GLenum) (GL_COLOR_ATTACHMENT0 + colAttachment);
    colourAttachments.push_back(attachment);
    if (textures[textureID])
      glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textures[textureID]->glTexType, textures[textureID]->glTexID, 0);
    else
      success = false;
    ASSERT_NO_GL_ERROR();
    }

  if (success)
    {
    glDrawBuffers((GLsizei) colourAttachments.size(), colourAttachments.data());
    success = checkFBOStatus();
    }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return success;
  }

bool mathernogl::FrameBuffer::initialiseForShadowMapping(uint width, uint height)
  {
  fboWidth = width;
  fboHeight = height;
  clearGLErrors();
  glGenFramebuffers(1, &fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, fboID);
  ASSERT_NO_GL_ERROR();

  clearGLErrors();
  textures[textureDepth].reset(createShadowMapDepthTexture(width, height));
  if (textures[textureDepth])
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[textureDepth]->glTexType, textures[textureDepth]->glTexID, 0);
  ASSERT_NO_GL_ERROR();

  bool success = false;
  if (textures[textureDepth])
    success = checkFBOStatus();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return success;
  }

void mathernogl::FrameBuffer::cleanUp()
  {
  glDeleteFramebuffers(1, &fboID);
  }

const std::shared_ptr<mathernogl::Texture> mathernogl::FrameBuffer::getColourTexture(int idx) const
  {
  return textures[textureColour0 + idx];
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
//  glDrawBuffer(GL_BACK);
  glBlitFramebuffer(0, 0, fboWidth, fboHeight, 0, 0, fbo->getWidth(), fbo->getHeight(), buffersToBlit, GL_NEAREST);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  }


