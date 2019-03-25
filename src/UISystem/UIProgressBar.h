#pragma once

#include "UIPanel.h"

/*
*   
*/

class UIProgressBar : public UIPanel
  {
private:
  std::shared_ptr<UIPanel> barComponent;
  double progressValue = 0;
  Vector3D barColour;
  uint barPadding = 2;

public:
  UIProgressBar(uint id);
  void setProgress(double value);
  double getProgress() const { return progressValue; }
  void setBarColour(const Vector3D& colour);
  void setBarPadding(uint padding);
  virtual void initialise(GameContext* context) override;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;

  //  changes bar value and calls invalidate, only if bar value is different to current
  //  returns true if bar progress was changed
  static bool updateProgressBar(UIProgressBar* bar, double value, double max);
  };
