//
// Created by matt on 1/02/18.
//

#include "RenderableLines.h"

RenderableLines::RenderableLines(uint id, int drawStage) : Renderable(id, drawStage)
  {}

void RenderableLines::initialise(RenderContext* renderContext)
  {
  using namespace mathernogl;
  clearGLErrors();
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/LinesVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/LinesFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);

  vbo.init();
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(vbo, 0,                 4 * sizeof(float), 3, 0, false);
  vao.linkBufferAsFloats(vbo, 3 * sizeof(float), 3 * sizeof(float), 4, 1, false);

  glEnable(GL_PROGRAM_POINT_SIZE);
  ASSERT_NO_GL_ERROR()
  }

void RenderableLines::cleanUp(RenderContext* renderContext)
  {
  vao.cleanUp();
  vbo.cleanUp();
  }

void RenderableLines::render(RenderContext* renderContext)
  {
  using namespace mathernogl;
  clearGLErrors();
  setDepthTest(true);
  setAlphaBlending(true);
  glLineWidth(lineWidth);
  renderContext->activateShaderProgram(shaderProgram);
  vao.bind();
  glDrawArrays(GL_LINES, 0, lines.count() * 2);
  ASSERT_NO_GL_ERROR();
  }

uint RenderableLines::addLine(const Vector3D& start, const Vector3D& end, const Vector3D& colour, const float alpha)
  {
  uint id = nextLineID++;
  lines.add({start, end, colour, alpha}, id);
  rebuildBuffer();
  return id;
  }

void RenderableLines::removeLine(uint lineID)
  {
  if (lines.contains(lineID))
    {
    lines.remove(lineID);
    rebuildBuffer();
    }
  }

void RenderableLines::rebuildBuffer()
  {
  if (rebuildDisabled)
    return;

  std::vector<float> data;
  for (const Line& line : *lines.getList())
    {
    data.push_back((float)line.start.x);
    data.push_back((float)line.start.y);
    data.push_back((float)line.start.z);
    data.push_back((float)line.colour.x);
    data.push_back((float)line.colour.y);
    data.push_back((float)line.colour.z);
    data.push_back(line.alpha);

    data.push_back((float)line.end.x);
    data.push_back((float)line.end.y);
    data.push_back((float)line.end.z);
    data.push_back((float)line.colour.x);
    data.push_back((float)line.colour.y);
    data.push_back((float)line.colour.z);
    data.push_back(line.alpha);
    }
  vbo.bind();
  vbo.copyDataFloat(data);
  }

void RenderableLines::clearLines()
  {
  lines.clear();
  rebuildBuffer();
  }