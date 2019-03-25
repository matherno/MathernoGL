#pragma once

#include <Types.h>
#include <texture/Texture.h>
#include <vector>
#include <maths/Vector2D.h>

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
  const std::shared_ptr<Texture> getColourTexture(int idx = 0) const;
  const std::shared_ptr<Texture> getDepthTexture() const;
  bool initialise(uint width, uint height, bool needAlphaChannel, bool useDepthTexture, bool gammaCorrect, int numColourOutputs = 1);
  bool initialiseMultisampled(uint width, uint height, uint numSamples, bool gammaCorrect, int numColourOutputs = 1);
  bool initialiseForShadowMapping(uint width, uint height);
  void cleanUp();
  void clear();
  void blitToScreen(uint screenWidth, uint screenHeight);
  void blitToFBO(FrameBuffer* fbo, bool includeDepth, bool includeStencil);

  uint getGLID() const { return fboID; }
  uint getWidth() const { return fboWidth; }
  uint getHeight() const { return fboHeight; }
  };

  }