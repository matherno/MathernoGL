#pragma once

#include "RenderSystem.h"
#include "RenderableLines.h"

/*
*   
*/

class RenderableLineBoxes : public Renderable
  {
private:
  std::shared_ptr<RenderableLines> linesRenderable;
  std::map<uint, std::vector<uint>> boxToLineIDs;
  uint nextBoxID = 1;
  Vector3D lineColour;
  float alpha = 1;
  float lineWidth = 2;

public:
  RenderableLineBoxes(uint id, int drawStage = DRAW_STAGE_OPAQUE);

  uint addBox(const Vector3D& min, const Vector3D& max);
  void removeBox(uint id);
  void clearBoxes();
  void setLineColour(const Vector3D& colour){ lineColour = colour; }
  void setAlpha(float alpha){ this->alpha = alpha; }
  void setLineWidth(float width) { lineWidth = width; }

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  typedef std::pair<Vector3D, Vector3D> Line;
  static void constructLineBox(std::vector<Line>* lines, const Vector3D& min, const Vector3D& max);
  };
