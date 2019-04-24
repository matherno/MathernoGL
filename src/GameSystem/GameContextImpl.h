#pragma once

#include <list>
#include <map>
#include "GameSystem.h"
#include "InputManagerImpl.h"
#include "Camera.h"
#include "BoundingBoxManagerImpl.h"
#include <RenderSystem/RenderContextImpl.h>
#include <set>
#include <UISystem/UIManagerImpl.h>

class GameContextImpl : public GameContext
  {
enum ProcessStage
  {
  stageNone,
  stageInit,
  stageInput,
  stageUpdate,
  stageRender,
  stageCleanUp,
  };

private:
  InputManagerImpl inputManager;
  UIManagerImpl uiManager;
  uint nextActorID = 1;
  mathernogl::MappedList<GameActorPtr> actors;
  Camera camera;
  long startFrameTime;
  long gameTime = 0;
  long deltaTime = 0;
  ProcessStage stage = stageNone;
  std::set<uint> actorsToRemove;
  bool paused = false;
  bool shouldEnd = false;
  BoundingBoxManagerImpl boundingBoxManager;
  uint speed = 1;
  RenderablePtr imGuiRenderable;

public:
  GameContextImpl(RenderContextPtr renderContext) : GameContext(renderContext) {}
  virtual ~GameContextImpl() override {}

  virtual bool initialise() override;
  virtual void cleanUp() override;

  virtual void addActor(GameActorPtr actor) override;
  virtual void removeActor(uint id) override;
  virtual int getNumActors() override;
  virtual bool gotActor(uint id) override;
  virtual uint getNextActorID() override;

  virtual void addInputHandler(InputHandlerPtr handler, bool priorityHandler = false) override;
  virtual void removeInputHandler(InputHandlerPtr handler) override;

  virtual long getGameTime() override { return gameTime; }
  virtual long getDeltaTime() override { return deltaTime; }
  virtual void startFrame() override;
  virtual void processInputStage() override;
  virtual void processUpdateStage() override;
  virtual void processDrawStage() override;
  virtual void endFrame(float maxFPS) override;
  virtual void setPaused(bool pause) override;
  virtual bool isPaused() const override;
  virtual void endContext() override { shouldEnd = true; }
  virtual bool isContextEnded() const override { return shouldEnd; }

  virtual Camera* getCamera() override { return &camera; }
  virtual InputManager* getInputManager() override { return &inputManager; }
  virtual UIManager* getUIManager() override { return &uiManager; }
  virtual BoundingBoxManager* getBoundingBoxManager() override { return &boundingBoxManager; } ;
  virtual const Camera* getCamera() const override { return &camera; }
  virtual const InputManager* getInputManager() const override { return &inputManager; }
  virtual const UIManager* getUIManager() const override { return &uiManager; }
  virtual const BoundingBoxManager* getBoundingBoxManager() const override { return &boundingBoxManager; } ;
  virtual FontPtr getDefaultFont() override;

  virtual Vector3D getCursorWorldPos(uint cursorX, uint cursorY) const override;
  virtual Vector3D clipToWorldTransform(const Vector3D& clipSpacePos) const override;
  virtual Vector3D getViewDirection() const override;
  virtual Vector3D getViewDirectionAtCursor(uint cursorX, uint cursorY) const override;

  virtual void setSpeed(uint speed) override { this->speed = speed; }
  virtual uint getSpeed() const override { return speed; }

  virtual void doIMGui() override;

protected:
  void removePendingActors();
  };

class BBInputHandler : public InputHandler
  {
public:
  BBInputHandler(uint id) : InputHandler(id) {}
  virtual void onAttached(GameContext* gameContext) override {}
  virtual void onDetached(GameContext* gameContext) override {}
  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  };