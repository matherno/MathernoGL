#pragma once

#include <mathernogl/Types.h>
#include <mathernogl/texture/Texture.h>
#include <vector>
#include <mathernogl/maths/Vector2D.h>

/*
*   
*/

namespace mathernogl
  {

class FrameBuffer
  {
private:
  uint fboID = 0;
  std::vector<std::shared_ptr<Texture>> textures;
  uint fboWidth = 0;
  uint fboHeight = 0;

public:
  FrameBuffer();
  const std::shared_ptr<Texture> getColourTexture() const;
  const std::shared_ptr<Texture> getDepthTexture() const;
  bool initialise(uint width, uint height, bool needAlphaChannel, bool useDepthTexture, bool gammaCorrect);
  bool initialiseMultisampled(uint width, uint height, uint numSamples, bool gammaCorrect);
  void cleanUp();
  void clear();
  void blitToScreen(uint screenWidth, uint screenHeight);
  void blitToFBO(FrameBuffer* fbo, bool includeDepth, bool includeStencil);

  uint getGLID() const { return fboID; }
  uint getWidth() const { return fboWidth; }
  uint getHeight() const { return fboHeight; }
  };

  }