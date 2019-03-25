//
// Created by matherno on 17/11/17.
//

#include "RenderableSetImpl.h"
#include "RenderUtils.h"

RenderableSetImpl::RenderableSetImpl(uint id) : RenderableSet(id)
  {
  }

void RenderableSetImpl::initialise(RenderContext* renderContext)
  {
  }

void RenderableSetImpl::cleanUp(RenderContext* renderContext)
  {
  }

void RenderableSetImpl::render(RenderContext* renderContext)
  {
  render(renderContext, false);
  }

void RenderableSetImpl::renderShadowMap(RenderContext* renderContext)
  {
  render(renderContext, true);
  }

void RenderableSetImpl::addRenderable(RenderablePtr renderable)
  {
  if (!containsRenderable(renderable->getID()))
    {
    renderables.add(renderable, renderable->getID());
    }
  }

void RenderableSetImpl::removeRenderable(int id)
  {
  renderables.remove(id);
  }

bool RenderableSetImpl::containsRenderable(int id) const
  {
  return renderables.contains(id);
  }

int RenderableSetImpl::count() const
  {
  return renderables.count();
  }

void RenderableSetImpl::forEachChild(std::function<void(RenderablePtr)> function)
  {
  for (RenderablePtr& child : *renderables.getList())
    function(child);
  }

void RenderableSetImpl::render(RenderContext* renderContext, bool shadowMap)
  {
  const Vector4D* clipPlane = getClippingPlane();
  bool usingParentClipPlane = clipPlane->x != 0 || clipPlane->y != 0 || clipPlane->z != 0;
  const int activeDrawStage = renderContext->getActiveDrawStage();

  for (RenderablePtr renderable : *renderables.getList())
    {
    if (renderable->getDrawStage() == DRAW_STAGE_ALL || renderable->getDrawStage() == activeDrawStage)
      {
      BoundingBoxPtr childBounds = renderable->getBounds();
      if (childBounds)
        {
        BoundingBoxPtr transformedBounds = std::make_shared<BoundingBox>(*childBounds);
        RenderUtils::transformBoundingBox(transformedBounds.get(), getTransform());
        if (RenderUtils::isBoundingBoxClipped(transformedBounds.get(), renderContext->getWorldToClip()))
          continue;
        }

      if (!usingParentClipPlane)
        renderContext->setClippingPlane(*renderable->getClippingPlane());

      renderContext->pushTransform(renderable->getTransform());
      if (shadowMap)
        renderable->renderShadowMap(renderContext);
      else
        renderable->render(renderContext);
      renderContext->popTransform();

      if (!usingParentClipPlane)
        renderContext->disableClippingPlane();
      }
    }
  }

BoundingBoxPtr RenderableSetImpl::getBounds()
  {
  if (renderables.count() == 0)
    return nullptr;

  BoundingBoxPtr bounds(new BoundingBox());
  for (RenderablePtr renderable : *renderables.getList())
    {
    BoundingBoxPtr childBounds = renderable->getBounds();
    if (!childBounds)
      return nullptr;
    bounds->addBoundingBox(childBounds);
    }
  RenderUtils::transformBoundingBox(bounds.get(), getTransform());
  return bounds;
  }
