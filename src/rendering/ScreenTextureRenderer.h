#pragma once

/*
*   
*/

#include <texture/Texture.h>
#include <data/VertexArray.h>
#include <memory>
#include "ShaderProgram.h"

namespace mathernogl
  {

class ScreenTextureRenderer
  {
private:
  const bool uniqueRenderer;
  VertexArray vao;
  ShaderProgram shaderProgram;
  std::shared_ptr<Texture> texture;
  uint textureBoundLocation = 4;

public:
  //  uniqueRenderer => if false, will bind vao, bind texture, and enable shader every render call
  ScreenTextureRenderer(bool uniqueRenderer) : uniqueRenderer(uniqueRenderer) {}

  void setTexture(std::shared_ptr<Texture> texture);
  void setTextureBindingLocation(uint local) { textureBoundLocation = local; }
  void init();
  void render();
  void cleanUp();
  };

  }