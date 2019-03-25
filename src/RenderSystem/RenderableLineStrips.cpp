//
// Created by matt on 25/02/18.
//

#include "RenderableLineStrips.h"

RenderableLineStrips::RenderableLineStrips(uint id, bool screenSpace, int drawStage) : Renderable(id, drawStage), inScreenSpace(screenSpace)
  {}

void RenderableLineStrips::initialise(RenderContext* renderContext)
  {
  using namespace mathernogl;
  clearGLErrors();

  std::vector<Shader> shaders;
  if (inScreenSpace)
    shaders = { Shader(GL_VERTEX_SHADER, "shaders/ScreenLineStripVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/ScreenLineStripFS.glsl") };
  else
    shaders = { Shader(GL_VERTEX_SHADER, "shaders/LineStripVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/LineStripFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);

  vbo.init();
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(vbo, 3, 0, false);
  glEnable(GL_PROGRAM_POINT_SIZE);
  ASSERT_NO_GL_ERROR()
  screenSize = renderContext->getWindow()->getSize();
  }

void RenderableLineStrips::cleanUp(RenderContext* renderContext)
  {
  vao.cleanUp();
  vbo.cleanUp();
  }

void RenderableLineStrips::render(RenderContext* renderContext)
  {
  using namespace mathernogl;
  clearGLErrors();
  setDepthTest(true);
  setAlphaBlending(true);
  glLineWidth(lineWidth);
  renderContext->activateShaderProgram(shaderProgram);
  vao.bind();
  uint bufferOffset = 0;
  for (const LineStrip& lineStrip : *lineStrips.getList())
    {
    shaderProgram->setVarVec4("inColour", Vector4D(lineStrip.colour, lineStrip.alpha));
    glDrawArrays(GL_LINE_STRIP, bufferOffset, (uint)lineStrip.points.size());
    bufferOffset += lineStrip.points.size();
    }
  ASSERT_NO_GL_ERROR();
  }

void RenderableLineStrips::startLineStrip(const Vector3D& colour, float alpha)
  {
  pendingLineStrip.points.clear();
  pendingLineStrip.colour = colour;
  pendingLineStrip.alpha = alpha;
  }

void RenderableLineStrips::addPoint(const Vector3D& point)
  {
  if (inScreenSpace)
    {
    Vector3D screenPoint = point;
    screenPoint.x /= screenSize.x;
    screenPoint.y /= screenSize.y;
    screenPoint *= 2;
    screenPoint -= 1;
    screenPoint.y *= -1;
    screenPoint.z = -1;
    pendingLineStrip.points.push_back(screenPoint);
    }
  else
    {
    pendingLineStrip.points.push_back(point);
    }
  }

uint RenderableLineStrips::finishLineStrip()
  {
  if (pendingLineStrip.points.size() < 2)
    return 0;

  uint id = nextLineStripID++;
  lineStrips.add(pendingLineStrip, id);
  rebuildBuffer();
  return id;
  }

void RenderableLineStrips::removeLineStrip(uint lineStripID)
  {
  if (lineStrips.contains(lineStripID))
    {
    lineStrips.remove(lineStripID);
    rebuildBuffer();
    }
  }

void RenderableLineStrips::clearLineStrips()
  {
  lineStrips.clear();
  rebuildBuffer();
  }

void RenderableLineStrips::rebuildBuffer()
  {
  if (rebuildDisabled)
    return;

  std::vector<float> data;
  for (const LineStrip& lineStrip : *lineStrips.getList())
    {
    for (const Vector3D& point : lineStrip.points)
      {
      data.push_back((float)point.x);
      data.push_back((float)point.y);
      data.push_back((float)point.z);
      }
    }
  vbo.bind();
  vbo.copyDataFloat(data);
  }

void RenderableLineStrips::addXZPlaneCircle(const Vector3D& centre, float radius, uint numSectors)
  {
  static const float TWO_PI = 2.0f * (float)M_PI;
  float deltaAngle = TWO_PI / numSectors;
  for (int ptNum = 0; ptNum <= numSectors; ++ptNum)
    {
    float angle = deltaAngle * ptNum;
    addPoint(centre + Vector3D(cosf(angle), 0, sinf(angle)) * radius);
    }
  }
