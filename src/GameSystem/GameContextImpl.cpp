//
// Created by matherno on 22/11/17.
//

#include "GameContextImpl.h"

bool GameContextImpl::initialise()
  {
  stage = stageInit;
  inputManager.initialise(getRenderContext()->getWindow());
  boundingBoxManager.initialise();
  addInputHandler(InputHandlerPtr(new BBInputHandler(inputManager.getNextHandlerID())), true);
  uiManager.initialise(this);
  gameTime = 0;
  stage = stageNone;
  return true;
  }

void GameContextImpl::cleanUp()
  {
  stage = stageCleanUp;
  for (GameActorPtr actor : *actors.getList())
    actor->onDetached(this);
  actors.clear();
  actorsToRemove.clear();
  boundingBoxManager.cleanUp();
  uiManager.cleanUp(this);
  stage = stageNone;
  }

void GameContextImpl::addActor(GameActorPtr actor)
  {
  actors.add(actor, actor->getID());
  actor->onAttached(this);
  }

void GameContextImpl::removeActor(uint id)
  {
  if (stage != stageUpdate && stage != stageCleanUp)
    {
    if (actors.contains(id))
      {
      GameActorPtr actor = actors.get(id);
      actors.remove(id);
      actor->onDetached(this);
      }
    }
  else
    {
    actorsToRemove.emplace(id);
    }
  }

int GameContextImpl::getNumActors()
  {
  return actors.count();
  }

bool GameContextImpl::gotActor(uint id)
  {
  return actors.contains(id);
  }

uint GameContextImpl::getNextActorID()
  {
  return nextActorID++;
  }

void GameContextImpl::processInputStage()
  {
  stage = stageInput;
  boundingBoxManager.clearMousePickingResult();
  inputManager.processInput(this);
  stage = stageNone;
  }

void GameContextImpl::processUpdateStage()
  {
  stage = stageUpdate;
  if (!paused && !getUIManager()->isModalModeActive())
    {
    for (int i = 0; i < speed; ++i)
      {
      for (GameActorPtr actor : *actors.getList())
        actor->onUpdate(this);
      }
    }
  uiManager.update(this);
  stage = stageNone;
  removePendingActors();
  }

void GameContextImpl::processDrawStage()
  {
  stage = stageRender;
  if (!camera.isValid())
    {
    getRenderContext()->setWorldToCamera(*camera.getWorldToCamera()->getTransformMatrix());
    getRenderContext()->setCameraToClip(*camera.getCameraToClip()->getTransformMatrix());
    camera.setValid(true);
    }

  getRenderContext()->render();
  stage = stageNone;
  }

void GameContextImpl::addInputHandler(InputHandlerPtr handler, bool priorityHandler)
  {
  if (priorityHandler)
    inputManager.addPriorityHandler(handler);
  else
    inputManager.addHandler(handler);
  handler->onAttached(this);
  }

void GameContextImpl::removeInputHandler(InputHandlerPtr handler)
  {
  inputManager.removeHandler(handler->getID());
  handler->onDetached(this);
  }

void GameContextImpl::startFrame()
  {
  if (!paused)
    gameTime += deltaTime * speed;
  startFrameTime = mathernogl::getTimeMS();
  }

void GameContextImpl::endFrame(float maxFPS)
  {
  long minTimeBetweenFrames = (long)((1.0 / (double)maxFPS) * 1000);
  while (mathernogl::getTimeMS() - startFrameTime < minTimeBetweenFrames)
    {
    }
  deltaTime = mathernogl::getTimeMS() - startFrameTime;
  }

void GameContextImpl::removePendingActors()
  {
  std::for_each(actorsToRemove.begin(), actorsToRemove.end(), [this](uint id)
    {
    removeActor(id);
    });
  actorsToRemove.clear();
  }

void GameContextImpl::setPaused(bool pause)
  {
  paused = pause;
  }

bool GameContextImpl::isPaused() const
  {
  return paused;
  }

Vector3D GameContextImpl::getCursorWorldPos(uint cursorX, uint cursorY) const
  {
  uint screenWidth = getRenderContext()->getWindow()->getWidth();
  uint screenHeight = getRenderContext()->getWindow()->getHeight();
  Vector3D clipSpacePos = Vector3D((float)cursorX / (float)screenWidth * 2.0f - 1.0f, (1.0f - (float)cursorY / (float)screenHeight) * 2.0f - 1.0f, 0);
  return clipToWorldTransform(clipSpacePos);
  }

Vector3D GameContextImpl::getViewDirection() const
  {
  return (clipToWorldTransform(Vector3D(0, 0, 1)) - clipToWorldTransform(Vector3D(0, 0, 0))).getUniform();
  }

Vector3D GameContextImpl::getViewDirectionAtCursor(uint cursorX, uint cursorY) const
  {
  uint screenWidth = getRenderContext()->getWindow()->getWidth();
  uint screenHeight = getRenderContext()->getWindow()->getHeight();
  Vector3D clipSpacePos = Vector3D((float)cursorX / (float)screenWidth * 2.0f - 1.0f, (1.0f - (float)cursorY / (float)screenHeight) * 2.0f - 1.0f, 0);
  const bool isOrthoCam = getRenderContext()->getCameraToClip()->getAt(3, 3) > 0.0;
  if (isOrthoCam)
    return (clipToWorldTransform(clipSpacePos - Vector3D(0, 0, 1)) - clipToWorldTransform(clipSpacePos)).getUniform();
  else
    return (clipToWorldTransform(clipSpacePos + Vector3D(0, 0, 1)) - clipToWorldTransform(clipSpacePos)).getUniform();
  }

Vector3D GameContextImpl::clipToWorldTransform(const Vector3D& clipSpacePos) const
  {
  using namespace mathernogl;
  return clipSpacePos * matrixInverse(*getRenderContext()->getCameraToClip()) * matrixInverse(*getRenderContext()->getWorldToCamera());
  }

FontPtr GameContextImpl::getDefaultFont()
  {
  return nullptr;
  }

bool BBInputHandler::onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  gameContext->getBoundingBoxManager()->performMousePicking(gameContext, mouseX, mouseY);
  return false;
  }

bool BBInputHandler::onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  gameContext->getBoundingBoxManager()->performMousePicking(gameContext, mouseX, mouseY);
  return false;
  }
