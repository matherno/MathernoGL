//
// Created by matt on 28/12/17.
//

#include <GameSystem/InputCodes.h>
#include "UIInputHandler.h"

#define DOUBLE_CLICK_TOLERANCE 200

UIInputHandler::UIInputHandler(uint id) : InputHandler(id)
  {}

void UIInputHandler::onAttached(GameContext* gameContext)
  {

  }

void UIInputHandler::onDetached(GameContext* gameContext)
  {

  }

bool UIInputHandler::onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  if (componentUnderMouse != 0)
    {
    if (button == MOUSE_LEFT)
      {
      if (gameContext->getGameTime() - timeLastClicked < DOUBLE_CLICK_TOLERANCE)
        {
        //  double click
        timeLastClicked = -20000;
        gameContext->getUIManager()->mouseClick(gameContext, mouseX, mouseY, true);
        }
      else
        {
        //  normal click
        timeLastClicked = gameContext->getGameTime();
        gameContext->getUIManager()->mouseClick(gameContext, mouseX, mouseY, false);
        }
      }
    return true;
    }
  else
    {
    gameContext->getUIManager()->lossFocus(gameContext);
    return false;
    }
  }

bool UIInputHandler::onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  return gameContext->getUIManager()->hitTest(mouseX, mouseY, false) != 0;
  }

bool UIInputHandler::onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  return gameContext->getUIManager()->hitTest(mouseX, mouseY, false) != 0;
  }

bool UIInputHandler::onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY)
  {
  return gameContext->getUIManager()->hitTest(mouseX, mouseY, false) != 0;
  }

bool UIInputHandler::onKeyPressed(GameContext* gameContext, uint key)
  {
  return gameContext->getUIManager()->keyPress(gameContext, key);
  }

bool UIInputHandler::onKeyRepeated(GameContext* gameContext, uint key)
  {
  return gameContext->getUIManager()->keyPress(gameContext, key);
  }

bool UIInputHandler::onMouseMove(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY)
  {
  uint hitComponentID = gameContext->getUIManager()->hitTest(mouseX, mouseY, true);
  if (componentUnderMouse != hitComponentID)
    {
    UIComponentPtr component = gameContext->getUIManager()->getComponent(componentUnderMouse, true);
    if (component)
      component->onMouseExit();

    component = gameContext->getUIManager()->getComponent(hitComponentID, true);
    if (component)
      component->onMouseEnter();
    componentUnderMouse = hitComponentID;
    }
  return false;
  }
