#pragma once

#include "UISystem.h"

/*
*   
*/

class UIManagerImpl : public UIManager
  {
private:
  uint nextComponentID = 1;
  UIComponentPtr rootComponent;
  std::list<UIComponentPtr> modalComponents;
  UIComponentPtr focusedComponent;

public:
  virtual void addComponent(UIComponentPtr component) override;
  virtual void removeComponent(uint id) override;
  virtual UIComponentPtr getComponent(uint id, bool recurseChildren = false) override;
  virtual uint getNextComponentID() override;
  virtual void pushModalComponent(UIComponentPtr modalComponent) override;
  virtual void popModalComponent() override;
  virtual bool isModalModeActive() const override;
  virtual bool mouseClick(GameContext* context, uint mouseX, uint mouseY, bool dblClick) override;
  virtual bool keyPress(GameContext* context, uint key) override;
  virtual bool initialise(GameContext* context) override;
  virtual void update(GameContext* context) override;
  virtual void cleanUp(GameContext* context) override;
  virtual uint hitTest(uint mouseX, uint mouseY, bool onlyCheckModal) override;
  virtual void lossFocus(GameContext* context) override;
  };
