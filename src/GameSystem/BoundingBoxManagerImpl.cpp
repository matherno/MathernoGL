//
// Created by matt on 11/02/18.
//

#include "BoundingBoxManagerImpl.h"

void BoundingBoxManagerImpl::initialise()
  {
  }

void BoundingBoxManagerImpl::cleanUp()
  {
  boundingBoxes.clear();
  }

uint BoundingBoxManagerImpl::addBoundingBox(const Vector3D& min, const Vector3D& max, double meta)
  {
  uint id = nextBBID++;
  BoundingBoxData newBoundingBox{id, meta, std::make_shared<BoundingBox>(min, max)};
  boundingBoxes.add(newBoundingBox, id);
  return id;
  }

uint BoundingBoxManagerImpl::addBoundingBox(BoundingBoxPtr box, double meta)
  {
  uint id = nextBBID++;
  BoundingBoxData newBoundingBox{id, meta, box};
  boundingBoxes.add(newBoundingBox, id);
  return id;
  }

void BoundingBoxManagerImpl::updateBoundingBox(uint id, const Vector3D& min, const Vector3D& max)
  {
  if (containsBoundingBox(id))
    {
    BoundingBoxPtr box = getBoundingBox(id);
    box->setBounds(min, max);
    }
  }

void BoundingBoxManagerImpl::removeBoundingBox(uint id)
  {
  if (containsBoundingBox(id))
    boundingBoxes.remove(id);
  }

bool BoundingBoxManagerImpl::containsBoundingBox(uint id)
  {
  return boundingBoxes.contains(id);
  }

void BoundingBoxManagerImpl::getBoundingBox(uint id, Vector3D* min, Vector3D* max)
  {
  if (containsBoundingBox(id))
    {
    BoundingBoxPtr box = getBoundingBox(id);
    *min = box->getLowerBound();
    *max = box->getUpperBound();
    }
  }

BoundingBoxPtr BoundingBoxManagerImpl::getBoundingBox(uint id)
  {
  if (containsBoundingBox(id))
    return boundingBoxes.get(id).boundingBox;
  }

bool BoundingBoxManagerImpl::performMousePicking(GameContext* gameContext, uint mouseX, uint mouseY)
  {
  Vector3D cursorWorldPos = gameContext->getCursorWorldPos(mouseX, mouseY);
  Vector3D cursorViewDir = gameContext->getViewDirectionAtCursor(mouseX, mouseY);
  Ray screenRay = Ray::create(cursorWorldPos, cursorViewDir);

  float minTValue;
  pickedBoundingBox = 0;
  pickedBoundingBoxMeta = 0;
  for (const BoundingBoxData& data : *boundingBoxes.getList())
    {
    BoundingBoxPtr box = data.boundingBox;
    double testHitTValue;
    if(box->hitTest(&screenRay, &testHitTValue))
      {
      if (pickedBoundingBox == 0 || testHitTValue < minTValue)
        {
        pickedBoundingBox = data.id;
        pickedBoundingBoxMeta = data.metaData;
        minTValue = testHitTValue;
        }
      }
    }

  return boundingBoxPicked();
  }

uint BoundingBoxManagerImpl::getPickedBoundingBox() const
  {
  return pickedBoundingBox;
  }

double BoundingBoxManagerImpl::getPickedBoundingBoxMeta() const
  {
  return pickedBoundingBoxMeta;
  }

bool BoundingBoxManagerImpl::boundingBoxPicked() const
  {
  return pickedBoundingBox > 0;
  }

