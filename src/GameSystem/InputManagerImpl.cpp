//
// Created by matherno on 22/11/17.
//

#include "InputManagerImpl.h"


void InputManagerImpl::initialise(mathernogl::Window* window)
  {
  inputSource.init(window->getGLFWWindow());
  prevMouseX = -1;
  prevMouseY = -1;
  }

void InputManagerImpl::cleanUp()
  {
  inputSource.cleanUp();
  }

void InputManagerImpl::addHandler(InputHandlerPtr handler)
  {
  handlers.addFront(handler, handler->getID());
  }

//  priority handlers are always called to handle input before the regular ones
void InputManagerImpl::addPriorityHandler(InputHandlerPtr handler)
  {
  priorityHandlers.addFront(handler, handler->getID());
  }

void InputManagerImpl::removeHandler(uint id)
  {
  if (handlers.contains(id))
    handlers.remove(id);
  if (priorityHandlers.contains(id))
    priorityHandlers.remove(id);
  }

bool InputManagerImpl::containsHandler(uint id) const
  {
  return handlers.contains(id) || priorityHandlers.contains(id);
  }

uint InputManagerImpl::getNextHandlerID()
  {
  return nextHandlerID++;
  }

bool InputManagerImpl::isKeyDown(uint key) const
  {
  return inputSource.isKeyAction(key, mathernogl::INPUT_HELD);
  }

bool InputManagerImpl::isMouseDown(uint button) const
  {
  return inputSource.isMouseButtonAction(button, mathernogl::INPUT_HELD);
  }

void InputManagerImpl::processInput(GameContext* gameContext)
  {
  inputSource.checkHeldButtons();

  //  fire key events

  for (const mathernogl::KeyEvent& event : *inputSource.getKeyEvents())
    {
    if (event.action == mathernogl::INPUT_PRESSED)
      fireKeyPressedEvents(gameContext, (uint) event.key);
    else if (event.action == mathernogl::INPUT_HELD)
      fireKeyHeldEvents(gameContext, (uint) event.key);
    else if (event.action == mathernogl::INPUT_RELEASED)
      fireKeyReleasedEvents(gameContext, (uint) event.key);
    else if (event.action == mathernogl::INPUT_REPEATED)
      fireKeyRepeatedEvents(gameContext, (uint) event.key);
    }

  Vector2D mouse = inputSource.getMousePos();
  uint mouseX = (uint) std::max(mouse.x, 0.0f);
  uint mouseY = (uint) std::max(mouse.y, 0.0f);

  //  fire mouse move events if the mouse has moved
  //  mouse move events should be fired before other mouse events
  if (prevMouseX >= 0 && prevMouseY >= 0 && (mouseX != prevMouseX || mouseY != prevMouseY))
    {
    fireMouseMoveEvents(gameContext, mouseX, mouseY, (uint)prevMouseX, (uint)prevMouseY);
    }
  prevMouseX = mouseX;
  prevMouseY = mouseY;

  //  fire mouse button events
  for (const mathernogl::MouseButtonEvent& event : *inputSource.getMouseButtonEvents())
    {
    if (event.action == mathernogl::INPUT_PRESSED)
      fireMousePressedEvents(gameContext, (uint) event.buttonNum, mouseX, mouseY);
    else if (event.action == mathernogl::INPUT_HELD)
      fireMouseHeldEvents(gameContext, (uint) event.buttonNum, mouseX, mouseY);
    else if (event.action == mathernogl::INPUT_RELEASED)
      fireMouseReleasedEvents(gameContext, (uint) event.buttonNum, mouseX, mouseY);
    }

  //  fire scroll wheel events if it was used
  double scrollWheelOffset = inputSource.getScrollWheelOffset();
  if (scrollWheelOffset != 0)
    fireMouseScrollEvents(gameContext, scrollWheelOffset, mouseX, mouseY);

  //  fire all update events
  fireUpdateEvents(gameContext);

  inputSource.clearEvents();
  }

bool shouldFire(GameContext* gameContext, InputHandlerPtr& handler)
  {
  if (gameContext->getUIManager()->isModalModeActive())
    return handler->isUIInputHandler();
  return true;
  }

void InputManagerImpl::fireUpdateEvents(GameContext* gameContext)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler))
      handler->onUpdate(gameContext);
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler))
      handler->onUpdate(gameContext);
    }
  }

void InputManagerImpl::fireMousePressedEvents(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMousePressed(gameContext, button, mouseX, mouseY))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMousePressed(gameContext, button, mouseX, mouseY))
      return;
    }
  }

void InputManagerImpl::fireMouseHeldEvents(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMouseHeld(gameContext, button, mouseX, mouseY))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMouseHeld(gameContext, button, mouseX, mouseY))
      return;
    }
  }

void InputManagerImpl::fireMouseReleasedEvents(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMouseReleased(gameContext, button, mouseX, mouseY))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMouseReleased(gameContext, button, mouseX, mouseY))
      return;
    }
  }

void InputManagerImpl::fireMouseScrollEvents(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMouseScroll(gameContext, scrollOffset, mouseX, mouseY))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMouseScroll(gameContext, scrollOffset, mouseX, mouseY))
      return;
    }
  }

void InputManagerImpl::fireMouseMoveEvents(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMouseMove(gameContext, mouseX, mouseY, prevMouseX, prevMouseY))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onMouseMove(gameContext, mouseX, mouseY, prevMouseX, prevMouseY))
      return;
    }
  }

void InputManagerImpl::fireKeyPressedEvents(GameContext* gameContext, uint key)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onKeyPressed(gameContext, key))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onKeyPressed(gameContext, key))
      return;
    }
  }

void InputManagerImpl::fireKeyHeldEvents(GameContext* gameContext, uint key)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onKeyHeld(gameContext, key))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onKeyHeld(gameContext, key))
      return;
    }
  }

void InputManagerImpl::fireKeyRepeatedEvents(GameContext* gameContext, uint key)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onKeyRepeated(gameContext, key))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onKeyRepeated(gameContext, key))
      return;
    }
  }

void InputManagerImpl::fireKeyReleasedEvents(GameContext* gameContext, uint key)
  {
  for (InputHandlerPtr& handler : *priorityHandlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onKeyReleased(gameContext, key))
      return;
    }
  for (InputHandlerPtr& handler : *handlers.getList())
    {
    if (shouldFire(gameContext, handler) && handler->onKeyReleased(gameContext, key))
      return;
    }
  }

Vector2D InputManagerImpl::getMousePos() const
  {
  return inputSource.getMousePos();
  }

