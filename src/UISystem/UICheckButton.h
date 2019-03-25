#pragma once

#include "UIButton.h"

/*
*   
*/

class UICheckButton : public UIButton
  {
private:
  Vector3D checkedColour;
  Vector3D uncheckedColour;

public:
  UICheckButton(uint id, bool isChecked);

  void setButtonColour(const Vector3D& checked);
  void setButtonColour(const Vector3D& checked, const Vector3D& unchecked);
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;

protected:
  virtual bool onButtonClick(uint mouseX, uint mouseY) override;
  virtual void onUnpress(uint mouseX, uint mouseY) override;
  virtual void onForceUnpress() override;
  };
