//
// Created by matt on 10/03/18.
//

#include <data/GPUBufferStatic.h>
#include "rendering/ScreenTextureRenderer.h"

static const std::string vertexShader =
  "#version 330\n"
  "\n"
  "layout(location = 0) in vec2 inPos;\n"
  "\n"
  "out vec2 texCoords;\n"
  "\n"
  "void main(){\n"
  "    texCoords = inPos;\n"
  "    vec2 clipPos = inPos;\n"
  "    clipPos *= 2;\n"
  "    clipPos -= 1;\n"
  "    gl_Position = vec4(clipPos, 0, 1);\n"
  "}";

static const std::string fragmentShader =
  "#version 330\n"
  "\n"
  "uniform sampler2D inTexture;\n"
  "\n"
  "in vec2 texCoords;\n"
  "\n"
  "out vec4 outputColour;\n"
  "\n"
  "void main(){\n"
  "   outputColour = texture(inTexture, texCoords);\n"
  "}";


namespace mathernogl
  {

void ScreenTextureRenderer::setTexture(std::shared_ptr<Texture> texture)
  {
  this->texture = texture;
  if (uniqueRenderer)
    {
    glActiveTexture(GL_TEXTURE0 + textureBoundLocation);
    glBindTexture(texture->glTexType, texture->glTexID);
    glActiveTexture(GL_TEXTURE0);
    }
  }

void ScreenTextureRenderer::init()
  {
  shaderProgram.initFromSrc(vertexShader, fragmentShader);
  shaderProgram.enable();
  shaderProgram.setVarInt("inTexture", textureBoundLocation);

  GPUBufferStatic vbo;
  vbo.init();
  vbo.bind();
  vbo.copyDataFloat(
    {
    0, 0,
    1, 1,
    0, 1,
    0, 0,
    1, 0,
    1, 1,
    });

  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(vbo, 2, 0, false);
  vao.unbind();
  vbo.cleanUp();
  vao.bind();
  }

void ScreenTextureRenderer::render()
  {
  if (!uniqueRenderer)
    {
    shaderProgram.enable();
    shaderProgram.setVarInt("inTexture", textureBoundLocation);
    vao.bind();
    glActiveTexture(GL_TEXTURE0 + textureBoundLocation);
    glBindTexture(texture->glTexType, texture->glTexID);
    glActiveTexture(GL_TEXTURE0);
    }
  glDrawArrays(GL_TRIANGLES, 0, 6);
  }

void ScreenTextureRenderer::cleanUp()
  {
  shaderProgram.cleanUp();
  vao.cleanUp();
  }

  }