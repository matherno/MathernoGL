#pragma once

#include <RenderSystem/RenderSystem.h>

/*
*   
*/

class UIRenderable : public Renderable
  {
private:
  ShaderProgramPtr shaderProgram;
  mathernogl::VertexArray vao;
  mathernogl::GPUBufferStatic vertBuffer;
  mathernogl::GPUBufferStatic texCoordBuffer;
  Vector3D colour;
  TexturePtr texture;
  bool alphaTexture = false;
  bool visible = true;
  Vector2D texCoordBL = Vector2D(0, 0);
  Vector2D texCoordTR = Vector2D(1, 1);
  bool initialised = false;
  float zDepth = 0;

public:
  UIRenderable(uint id);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void setVisible(bool visible){ this->visible = visible; }
  void refresh(const Vector2D& position, const Vector2D& size);
  void setColour(const Vector3D& colour);
  //  alpha texture means that it uses the assigned single colour and the textures alpha value for the final RGBA of the UI quad
  void setTexture(TexturePtr texture, bool alphaTexture){ this->texture = texture; this->alphaTexture = alphaTexture; }
  void setTextureCoords(const Vector2D& bottomLeft, const Vector2D& topRight);
  void setZDepth(float depth) { zDepth = depth; }

protected:
  void refreshTexCoords();
  };
