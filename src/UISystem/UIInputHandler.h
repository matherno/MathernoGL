#pragma once

#include <GameSystem/GameSystem.h>

/*
*   
*/

class UIInputHandler : public InputHandler
  {
private:
  long timeLastClicked = -20000;
  uint componentUnderMouse = 0;

public:
  UIInputHandler(uint id);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;
  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY) override;
  virtual bool onMouseMove(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY) override;
  virtual bool isUIInputHandler() const override { return true; }

  virtual bool onKeyPressed(GameContext* gameContext, uint key) override;
  virtual bool onKeyRepeated(GameContext* gameContext, uint key) override;
  };
