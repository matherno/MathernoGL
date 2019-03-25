//
// Created by matt on 28/12/17.
//

#include "UIRenderable.h"

using namespace mathernogl;

#define UI_DRAW_STYLE_SINGLE_COLOUR  1
#define UI_DRAW_STYLE_TEXTURE        2
#define UI_DRAW_STYLE_ALPHA_TEXTURE  3

UIRenderable::UIRenderable(uint id) : Renderable(id, DRAW_STAGE_UI)
  {}

void UIRenderable::initialise(RenderContext* renderContext)
  {
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/UserInterfaceVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/UserInterfaceFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);

  vertBuffer.init();
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(vertBuffer, 2, 0, false);

  texCoordBuffer.init();
  refreshTexCoords();
  vao.linkBufferAsFloats(texCoordBuffer, 2, 1, false);
  vao.unbind();

  initialised = true;
  }

void UIRenderable::cleanUp(RenderContext* renderContext)
  {
  vao.cleanUp();
  vertBuffer.cleanUp();
  texCoordBuffer.cleanUp();
  }

void UIRenderable::render(RenderContext* renderContext)
  {
  if (!visible)
    return;

  clearGLErrors();
  vao.bind();
  renderContext->activateShaderProgram(shaderProgram);
  setFaceCulling(false);
  setDepthTest(true);
  setAlphaBlending(true);
  shaderProgram->setVarFloat("inZDepth", zDepth);
  shaderProgram->setVarVec3("inColour", colour);
  if (texture)
    {
    shaderProgram->setVarInt("inDrawStyle", alphaTexture ? UI_DRAW_STYLE_ALPHA_TEXTURE : UI_DRAW_STYLE_TEXTURE);
    shaderProgram->setVarInt("inTexture", renderContext->bindTexture(texture));
    }
  else
    shaderProgram->setVarInt("inDrawStyle", UI_DRAW_STYLE_SINGLE_COLOUR);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  ASSERT_NO_GL_ERROR();
  }

void UIRenderable::refresh(const Vector2D& position, const Vector2D& size)
  {
  vertBuffer.copyDataFloat(
    {
      position.x,           position.y,
      position.x + size.x,  position.y,
      position.x + size.x,  position.y + size.y,
      position.x,           position.y,
      position.x,           position.y + size.y,
      position.x + size.x,  position.y + size.y,
    });
  }

void UIRenderable::setTextureCoords(const Vector2D& bottomLeft, const Vector2D& topRight)
  {
  texCoordBL = bottomLeft;
  texCoordTR = topRight;
  if (initialised)
    refreshTexCoords();
  }

void UIRenderable::refreshTexCoords()
  {
  Vector2D& bottomLeft = texCoordBL;
  Vector2D& topRight = texCoordTR;
  texCoordBuffer.copyDataFloat(
    {
      bottomLeft.x, topRight.y,
      topRight.x,   topRight.y,
      topRight.x,   bottomLeft.y,
      bottomLeft.x, topRight.y,
      bottomLeft.x, bottomLeft.y,
      topRight.x,   bottomLeft.y
    });
  }

void UIRenderable::setColour(const Vector3D& colour)
  {
  this->colour.x = pow(colour.x, 2.2);
  this->colour.y = pow(colour.y, 2.2);
  this->colour.z = pow(colour.z, 2.2);
  }
