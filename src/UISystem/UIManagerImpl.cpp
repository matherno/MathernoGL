//
// Created by matt on 27/12/17.
//

#include <GameSystem/InputCodes.h>
#include "UIManagerImpl.h"
#include "UIPanel.h"
#include "UIInputHandler.h"

#define RENDER_DEPTH_INCREMENT 0.001f

void UIManagerImpl::addComponent(UIComponentPtr component)
  {
  rootComponent->addChild(component);
  }

void UIManagerImpl::removeComponent(uint id)
  {
  rootComponent->removeChild(id);
  }

bool UIManagerImpl::initialise(GameContext* context)
  {
  rootComponent.reset(new UIPanel(getNextComponentID()));
  rootComponent->setHeightMatchParent(true);
  rootComponent->setWidthMatchParent(true);
  rootComponent->initialise(context);
  rootComponent->refresh(context, Vector2D(0, 0), context->getRenderContext()->getWindow()->getSize());
  rootComponent->setVisible(false);
  UIInputHandler* inputHandler = new UIInputHandler(context->getInputManager()->getNextHandlerID());
  context->addInputHandler(InputHandlerPtr(inputHandler), true);
  return true;
  }

void UIManagerImpl::update(GameContext* context)
  {
  rootComponent->onUpdate(context);
  float depth = -0.9f;
  rootComponent->setRenderDepth(&depth, RENDER_DEPTH_INCREMENT);
  }

void UIManagerImpl::cleanUp(GameContext* context)
  {
  rootComponent->cleanUp(context);
  }

UIComponentPtr UIManagerImpl::getComponent(uint id, bool recurseChildren)
  {
  return rootComponent->getChild(id, recurseChildren);
  }

uint UIManagerImpl::getNextComponentID()
  {
  return nextComponentID++;
  }

bool UIManagerImpl::mouseClick(GameContext* context, uint mouseX, uint mouseY, bool dblClick)
  {
  uint componentClicked = 0;
  UIComponentPtr componentToClick = isModalModeActive() ? (*modalComponents.begin()) : rootComponent;
  if (dblClick)
    componentClicked = componentToClick->mouseDblClick(context, mouseX, mouseY);
  else
    componentClicked = componentToClick->mouseClick(context, mouseX, mouseY);

  if (componentClicked == 0)
    {
    lossFocus(context);
    return false;
    }

  if (UIComponentPtr newFocusComponent = getComponent(componentClicked, true))
    {
    bool changingFocus = !focusedComponent || focusedComponent->getID() != newFocusComponent->getID();
    if (changingFocus)
      {
      lossFocus(context);
      focusedComponent = newFocusComponent;
      focusedComponent->onGainFocus(context);
      }
    }

  return true;
  }

bool UIManagerImpl::keyPress(GameContext* context, uint key)
  {
  if (key == KEY_ESC && isModalModeActive())
    {
    (*modalComponents.begin())->onEscapePressed(context);
    return true;
    }

  if (focusedComponent)
    return focusedComponent->keyPress(context, key);
  return false;
  }

void UIManagerImpl::lossFocus(GameContext* context)
  {
  if (focusedComponent)
    {
    focusedComponent->onLossFocus(context);
    focusedComponent.reset();
    }
  }

uint UIManagerImpl::hitTest(uint mouseX, uint mouseY, bool onlyCheckModal)
  {
  if (onlyCheckModal && isModalModeActive())
    return (*modalComponents.begin())->hitTest(mouseX, mouseY, true);
  return rootComponent->hitTest(mouseX, mouseY, true);
  }

void UIManagerImpl::pushModalComponent(UIComponentPtr modalComponent)
  {
  modalComponents.push_front(modalComponent);
  }

void UIManagerImpl::popModalComponent()
  {
  modalComponents.pop_front();
  }

bool UIManagerImpl::isModalModeActive() const
  {
  return !modalComponents.empty();
  }

//void UIManagerImpl::onMouseEnter(uint componentID)
//  {
//  UIComponentPtr component;
//  if (isModalModeActive())
//    {
//    UIComponentPtr topModalComponent = (*modalComponents.begin());
//    if (topModalComponent->getID() == componentID)
//      component = topModalComponent;
//    else
//      component = topModalComponent->getChild(componentID, true);
//    }
//  else
//    component = getComponent(componentID, true);
//
//  if (component)
//    component->onMouseEnter();
//  mouseOverComponent = component;
//  }
//
//void UIManagerImpl::onMouseExit(uint componentID)
//  {
//  if (mouseOverComponent)
//    {
//    UIComponentPtr component = getComponent(componentID, true);
//    if (component)
//      component->onMouseExit();
//    mouseOverComponent = nullptr;
//    }
//  }

