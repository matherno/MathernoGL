//
// Created by matt on 27/12/17.
//

#include "UIPanel.h"

UIPanel::UIPanel(uint id) : UIComponent(id)
  {}

void UIPanel::initialise(GameContext* context)
  {
  RenderContext* renderContext = context->getRenderContext();
  renderable.reset(new UIRenderable(renderContext->getNextRenderableID()));
  renderable->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(renderable);
  isValid = false;
  }

void UIPanel::onUpdate(GameContext* context)
  {
  removePendingComponents(context);
  addPendingComponents(context);
  refreshPendingComponents(context);
  for (UIComponentPtr& child : *children.getList())
    child->onUpdate(context);
  }

void UIPanel::cleanUp(GameContext* context)
  {
  RenderContext* renderContext = context->getRenderContext();
  renderable->cleanUp(renderContext);
  renderContext->getRenderableSet()->removeRenderable(renderable->getID());
  renderable.reset();
  for (UIComponentPtr& child : *children.getList())
    child->cleanUp(context);
  children.clear();
  mouseClickCallback = nullptr;   //  the callback function object may contain a reference to this (circular reference)
  isValid = false;
  }

void UIPanel::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  if (onRefreshCallback)
    onRefreshCallback(parentPos, parentSize);

  if (renderable)
    {
    Vector2D adjustedSize = size;
    Vector2D position = parentPos;

    if (widthMatchParent)
      {
      float padding = std::min(horizPadding, (parentSize.x * 0.5f) - 1.0f);
      adjustedSize.x = parentSize.x - (padding * 2);
      position.x += padding;
      }
    else
      {
      switch (horizAlignment)
        {
        case alignmentCentre:
          position.x += (parentSize.x * 0.5f) - (adjustedSize.x * 0.5);
          break;
        case alignmentEnd:
          position.x += parentSize.x - adjustedSize.x;
          break;
        }
      position.x += offset.x;
      }

    if (heightMatchParent)
      {
      float padding = std::min(vertPadding, (parentSize.y * 0.5f) - 1.0f);
      adjustedSize.y = parentSize.y - (padding * 2);
      position.y += padding;
      }
    else
      {
      switch (vertAlignment)
        {
        case alignmentCentre:
          position.y += (parentSize.y * 0.5f) - (adjustedSize.y * 0.5);
          break;
        case alignmentEnd:
          position.y += parentSize.y - adjustedSize.y;
          break;
        }
      position.y += offset.y;
      }

    adjustedSize.x = std::max(adjustedSize.x, 1.0f);
    adjustedSize.y = std::max(adjustedSize.y, 1.0f);

    renderable->setColour(colour);
    renderable->setTexture(texture, alphaTexture);
    renderable->setVisible(visible);
    renderable->setTextureCoords(texCoordBL, texCoordTR);
    renderable->refresh(position, adjustedSize);
    for (UIComponentPtr& child : *children.getList())
      child->refresh(context, position, adjustedSize);
    currentScreenPos = position;
    currentScreenSize = adjustedSize;
    isValid = true;
    }
  }

bool UIPanel::needsRefresh()
  {
  return !isValid;
  }

void UIPanel::invalidate()
  {
  isValid = false;
  }

void UIPanel::addChild(UIComponentPtr component)
  {
  childrenToAdd.push_back(component);
  }

void UIPanel::removeChild(uint id)
  {
  childrenToRemove.push_back(id);
  }

UIComponentPtr UIPanel::getChild(uint id, bool recurseChildren)
  {
  if (children.contains(id))
    return children.get(id);

  if (recurseChildren)
    {
    for (UIComponentPtr child : *children.getList())
      {
      if(UIComponentPtr comp = child->getChild(id, true))
        return comp;
      }
    }

  return nullptr;
  }

void UIPanel::refreshPendingComponents(GameContext* context)
  {
  for (UIComponentPtr comp : *children.getList())
    {
    if (comp->needsRefresh())
      comp->refresh(context, currentScreenPos, currentScreenSize);
    }
  }

void UIPanel::addPendingComponents(GameContext* context)
  {
  for (UIComponentPtr comp : childrenToAdd)
    {
    children.add(comp, comp->getID());
    comp->initialise(context);
    }
  childrenToAdd.clear();
  }

void UIPanel::removePendingComponents(GameContext* context)
  {
  for (uint id : childrenToRemove)
    {
    UIComponentPtr comp = children.get(id);
    if (comp)
      {
      children.remove(comp->getID());
      comp->cleanUp(context);
      }
    }
  childrenToRemove.clear();
  }

void UIPanel::setVisible(bool visible, bool recurseChildren)
  {
  this->visible = visible;
  if(renderable)
    renderable->setVisible(visible);
  if (recurseChildren)
    {
    for (UIComponentPtr child : *children.getList())
      child->setVisible(visible, recurseChildren);
    }
  }

uint UIPanel::mouseClick(GameContext* context, uint mouseX, uint mouseY)
  {
  for (UIComponentPtr comp : *children.getList())
    {
    uint id = comp->mouseClick(context, mouseX, mouseY);
    if (id != 0)
      return id;
    }

  if (mouseClickCallback && hitTest(mouseX, mouseY, false))
    {
    if (mouseClickCallback(mouseX, mouseY))
      return getID();
    }
  return 0;
  }

uint UIPanel::mouseDblClick(GameContext* context, uint mouseX, uint mouseY)
  {
  for (UIComponentPtr comp : *children.getList())
    {
    uint id = comp->mouseDblClick(context, mouseX, mouseY);
    if (id != 0)
      return id;
    }

  if (mouseDblClickCallback && hitTest(mouseX, mouseY, false))
    {
    if (mouseDblClickCallback(mouseX, mouseY))
      return getID();
    }
  return 0;
  }

uint UIPanel::hitTest(uint mouseX, uint mouseY, bool testChildren)
  {
  if (testChildren)
    {
    for (UIComponentPtr comp : *children.getList())
      {
      uint id = comp->hitTest(mouseX, mouseY, true);
      if (id > 0)
        return id;
      }
    }

  if (canHitWithMouse && visible && mouseX >= currentScreenPos.x && mouseX <= currentScreenPos.x + currentScreenSize.x)
    {
    if (mouseY >= currentScreenPos.y && mouseY <= currentScreenPos.y + currentScreenSize.y)
      return getID();
    }

  return 0;
  }

void UIPanel::setPadding(float horizPadding, float vertPadding)
  {
  this->horizPadding = horizPadding;
  this->vertPadding = vertPadding;
  }

void UIPanel::setRenderDepth(float* depth, float depthIncrement)
  {
  if (renderable)
    renderable->setZDepth(*depth);

  if (children.count() > 0)
    {
    *depth += depthIncrement;
    for (UIComponentPtr child : *children.getList())
      child->setRenderDepth(depth, depthIncrement);
    }
  }
