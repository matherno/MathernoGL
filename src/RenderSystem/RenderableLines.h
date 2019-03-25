#pragma once

#include "RenderSystem.h"

/*
*   
*/

class RenderableLines : public Renderable
  {
private:
  struct Line
    {
    Vector3D start;
    Vector3D end;
    Vector3D colour;
    float alpha;
    };

  ShaderProgramPtr shaderProgram;
  mathernogl::VertexArray vao;
  mathernogl::GPUBufferStatic vbo;
  mathernogl::MappedList<Line> lines;
  uint nextLineID = 0;
  bool rebuildDisabled = false;
  float lineWidth = 2;

public:
  RenderableLines(uint id, int drawStage = DRAW_STAGE_OPAQUE);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void setLineWidth(float lineWidth) {  this->lineWidth = lineWidth; }
  uint addLine(const Vector3D& start, const Vector3D& end, const Vector3D& colour, float alpha = 1.0f);
  void removeLine(uint lineID);
  void clearLines();

  void enableRebuild(){ rebuildDisabled = false; }
  void disableRebuild(){ rebuildDisabled = true; }
  void rebuildBuffer();
  };
