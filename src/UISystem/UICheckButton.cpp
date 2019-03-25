//
// Created by matt on 22/04/18.
//

#include "UICheckButton.h"

UICheckButton::UICheckButton(uint id, bool isChecked) : UIButton(id, true, isChecked)
  {
  setButtonHighlightColour(getColour(), getColour());
  setHighlightWidth(2);
  }

void UICheckButton::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  setButtonHighlightColour(getColour(), getColour());
  UIButton::refresh(context, parentPos, parentSize);
  }

void UICheckButton::setButtonColour(const Vector3D& checked)
  {
  checkedColour = checked;
  UIButton::setButtonColour(checkedColour);
  }

void UICheckButton::setButtonColour(const Vector3D& checked, const Vector3D& unchecked)
  {
  checkedColour = checked;
  uncheckedColour = unchecked;
  UIButton::setButtonColour(isToggledDown() ? checkedColour : uncheckedColour);
  }

bool UICheckButton::onButtonClick(uint mouseX, uint mouseY)
  {
  bool result = UIButton::onButtonClick(mouseX, mouseY);
  setButtonColour(checkedColour, uncheckedColour);
  return result;
  }

void UICheckButton::onUnpress(uint mouseX, uint mouseY)
  {
  UIButton::onUnpress(mouseX, mouseY);
  setButtonColour(checkedColour, uncheckedColour);
  }

void UICheckButton::onForceUnpress()
  {
  UIButton::onForceUnpress();
  setButtonColour(checkedColour, uncheckedColour);
  }
