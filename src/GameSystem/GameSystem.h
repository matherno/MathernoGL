#pragma once

#include <memory>
#include <RenderSystem/RenderSystem.h>
#include <UISystem/UISystem.h>
#include "Camera.h"
#include <RenderSystem/BoundingBox.h>

class GameContext;
class UIManager;

typedef std::function<void()> CallbackFunc;

/*
*   GameActor, an actor in the game representing an object with some behaviour and/or visual presence in the scene
*/
class GameActor
  {
private:
  const uint id;
public:
  GameActor(uint id) : id(id) {}
  virtual ~GameActor() {}
  uint getID() const { return id; }
  virtual void onAttached(GameContext* gameContext) = 0;
  virtual void onUpdate(GameContext* gameContext) = 0;
  virtual void onDetached(GameContext* gameContext) = 0;
  virtual void doIMGui(GameContext* gameContext) {};
  };
typedef std::shared_ptr<GameActor> GameActorPtr;


/*
*   InputHandler, an object ready to receive input from the player, and perform actions based on them
*       event functions return boolean, which indicates whether or not they handled and consumed the event
*/
class InputHandler
  {
private:
  const uint id;
public:
  InputHandler(uint id): id(id) {}
  virtual ~InputHandler() {}
  uint getID() { return id; }
  virtual void onAttached(GameContext* gameContext) = 0;
  virtual void onDetached(GameContext* gameContext) = 0;
  virtual void onUpdate(GameContext* gameContext) {};
  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) { return false; }
  virtual bool onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY) { return false; }
  virtual bool onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY) { return false; }
  virtual bool onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY) { return false; }
  virtual bool onMouseMove(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY) { return false; }
  virtual bool onKeyPressed(GameContext* gameContext, uint key) { return false; }
  virtual bool onKeyHeld(GameContext* gameContext, uint key) { return false; }
  virtual bool onKeyRepeated(GameContext* gameContext, uint key) { return false; }
  virtual bool onKeyReleased(GameContext* gameContext, uint key) { return false; }
  virtual bool isUIInputHandler() const { return false; }
  };
typedef std::shared_ptr<InputHandler> InputHandlerPtr;

/*
*   InputManager, responsible for managing a bunch of input handlers, and calling their event functions
*/
class InputManager
  {
public:
  InputManager(){}
  virtual ~InputManager(){}
  virtual void initialise(mathernogl::Window* window) = 0;
  virtual void cleanUp() = 0;
  virtual uint getNextHandlerID() = 0;
  virtual bool containsHandler(uint id) const = 0;
  virtual bool isKeyDown(uint key) const = 0;
  virtual bool isMouseDown(uint button) const = 0;
  virtual Vector2D getMousePos() const = 0;
protected:
  virtual void addHandler(InputHandlerPtr handler) = 0;
  virtual void addPriorityHandler(InputHandlerPtr handler) = 0;
  virtual void removeHandler(uint id) = 0;
  virtual void processInput(GameContext* gameContext) = 0;
  };

/*
*   BoundingBoxManager, used to handle all bound box operations
*   Bounding boxes each have an internally assigned unique ID,
*           and an optional meta data
*/
class BoundingBoxManager
  {
public:
  BoundingBoxManager(){};
  virtual ~BoundingBoxManager(){};
  virtual void initialise() = 0;
  virtual void cleanUp() = 0;
  virtual uint addBoundingBox(const Vector3D& min, const Vector3D& max, double meta = 0) = 0;
  virtual uint addBoundingBox(BoundingBoxPtr box, double meta = 0) = 0;
  virtual void updateBoundingBox(uint id, const Vector3D& min, const Vector3D& max) = 0;
  virtual void removeBoundingBox(uint id) = 0;
  virtual bool containsBoundingBox(uint id) = 0;
  virtual void getBoundingBox(uint id, Vector3D* min, Vector3D* max) = 0;
  virtual BoundingBoxPtr getBoundingBox(uint id) = 0;
  virtual bool performMousePicking(GameContext* gameContext, uint mouseX, uint mouseY) = 0;
  virtual uint getPickedBoundingBox() const = 0;
  virtual double getPickedBoundingBoxMeta() const = 0;
  virtual bool boundingBoxPicked() const = 0;
  };

/*
*   GameContext, the centralised game state and controller
*/
class GameContext
  {
private:
  RenderContextPtr renderContext;
public:
  GameContext(RenderContextPtr renderContext) : renderContext(renderContext) {};
  virtual ~GameContext(){};
  virtual bool initialise() = 0;
  virtual void cleanUp() = 0;
  virtual void addActor(GameActorPtr actor) = 0;
  virtual void removeActor(uint id) = 0;
  virtual int getNumActors() = 0;
  virtual uint getNextActorID() = 0;
  virtual bool gotActor(uint id) = 0;
  virtual void addInputHandler(InputHandlerPtr handler, bool priorityHandler = false) = 0;
  virtual void removeInputHandler(InputHandlerPtr handler) = 0;
  virtual long getGameTime() = 0;
  virtual long getDeltaTime() = 0;
  virtual void startFrame() = 0;
  virtual void processInputStage() = 0;
  virtual void processUpdateStage() = 0;
  virtual void processDrawStage() = 0;
  virtual void endFrame(float maxFPS) = 0;
  virtual void setPaused(bool pause) = 0;
  virtual bool isPaused() const = 0;
  virtual void endContext() = 0;
  virtual bool isContextEnded() const = 0;
  virtual Camera* getCamera() = 0;
  virtual InputManager* getInputManager() = 0;
  virtual RenderContext* getRenderContext() { return renderContext.get(); }
  virtual UIManager* getUIManager() = 0;
  virtual BoundingBoxManager* getBoundingBoxManager() = 0;
  virtual const Camera* getCamera() const = 0;
  virtual const InputManager* getInputManager() const = 0;
  virtual const RenderContext* getRenderContext() const { return renderContext.get(); };
  virtual const UIManager* getUIManager() const = 0;
  virtual const BoundingBoxManager* getBoundingBoxManager() const = 0;
  virtual FontPtr getDefaultFont() = 0;
  virtual Vector3D getCursorWorldPos(uint cursorX, uint cursorY) const = 0;
  virtual Vector3D clipToWorldTransform(const Vector3D& clipSpacePos) const = 0;
  virtual Vector3D getViewDirection() const = 0;
  virtual Vector3D getViewDirectionAtCursor(uint cursorX, uint cursorY) const = 0;
  virtual void setSpeed(uint speed) = 0;
  virtual uint getSpeed() const = 0;
  virtual void doIMGui() = 0;
  };


