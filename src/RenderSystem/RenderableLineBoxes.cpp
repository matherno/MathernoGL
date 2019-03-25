//
// Created by matt on 7/04/18.
//

#include "RenderableLineBoxes.h"

RenderableLineBoxes::RenderableLineBoxes(uint id, int drawStage) : Renderable(id, drawStage)
  {}

void RenderableLineBoxes::initialise(RenderContext* renderContext)
  {
  linesRenderable.reset(new RenderableLines(renderContext->getNextRenderableID()));
  linesRenderable->initialise(renderContext);
  linesRenderable->setLineWidth(lineWidth);
  }

void RenderableLineBoxes::cleanUp(RenderContext* renderContext)
  {
  boxToLineIDs.clear();
  linesRenderable->cleanUp(renderContext);
  }

void RenderableLineBoxes::render(RenderContext* renderContext)
  {
  linesRenderable->render(renderContext);
  }

uint RenderableLineBoxes::addBox(const Vector3D& min, const Vector3D& max)
  {
  std::vector<Line> boxLines;
  constructLineBox(&boxLines, min, max);
  uint id = nextBoxID++;
  for (Line line : boxLines)
    boxToLineIDs[id].emplace_back(linesRenderable->addLine(line.first, line.second, lineColour, alpha));
  return id;
  }

void RenderableLineBoxes::removeBox(uint id)
  {
  if (boxToLineIDs.count(id) > 0)
    {
    for (uint lineID : boxToLineIDs[id])
      linesRenderable->removeLine(lineID);
    boxToLineIDs.erase(id);
    }
  }

void RenderableLineBoxes::clearBoxes()
  {
  linesRenderable->clearLines();
  boxToLineIDs.clear();
  }

void RenderableLineBoxes::constructLineBox(std::vector<RenderableLineBoxes::Line>* lines, const Vector3D& min, const Vector3D& max)
  {
  Vector3D corners[] =
    {
      Vector3D(min.x, min.y, min.z),
      Vector3D(max.x, min.y, min.z),
      Vector3D(max.x, max.y, min.z),
      Vector3D(min.x, max.y, min.z),
      Vector3D(min.x, min.y, max.z),
      Vector3D(max.x, min.y, max.z),
      Vector3D(max.x, max.y, max.z),
      Vector3D(min.x, max.y, max.z),
    };

  //  min z face
  lines->emplace_back(corners[0], corners[1]);
  lines->emplace_back(corners[1], corners[2]);
  lines->emplace_back(corners[2], corners[3]);
  lines->emplace_back(corners[3], corners[0]);

  // max z face
  lines->emplace_back(corners[4], corners[5]);
  lines->emplace_back(corners[5], corners[6]);
  lines->emplace_back(corners[6], corners[7]);
  lines->emplace_back(corners[7], corners[4]);

  //  link the two faces
  lines->emplace_back(corners[0], corners[4]);
  lines->emplace_back(corners[1], corners[5]);
  lines->emplace_back(corners[2], corners[6]);
  lines->emplace_back(corners[3], corners[7]);
  }
