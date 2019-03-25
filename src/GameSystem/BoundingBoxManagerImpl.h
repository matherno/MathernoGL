#pragma once

#include "GameSystem/GameSystem.h"
#include "RenderSystem/BoundingBox.h"
#include "utils/MappedList.h"

/*
*   
*/


class BoundingBoxManagerImpl : public BoundingBoxManager
  {
private:
  struct BoundingBoxData
    {
    uint id;
    double metaData;
    BoundingBoxPtr boundingBox;
    };

  mathernogl::MappedList<BoundingBoxData> boundingBoxes;
  uint pickedBoundingBox = 0;
  double pickedBoundingBoxMeta = 0;
  uint nextBBID = 1;

public:
  virtual void initialise() override;
  virtual void cleanUp() override;

  virtual uint addBoundingBox(const Vector3D& min, const Vector3D& max, double meta = 0) override;
  virtual uint addBoundingBox(BoundingBoxPtr box, double meta = 0) override;
  virtual void updateBoundingBox(uint id, const Vector3D& min, const Vector3D& max) override;
  virtual void removeBoundingBox(uint id) override;
  virtual bool containsBoundingBox(uint id) override;
  virtual void getBoundingBox(uint id, Vector3D* min, Vector3D* max) override;
  virtual BoundingBoxPtr getBoundingBox(uint id) override;

  void clearMousePickingResult(){ pickedBoundingBox = 0; };
  virtual bool performMousePicking(GameContext* gameContext, uint mouseX, uint mouseY) override;
  virtual uint getPickedBoundingBox() const override;
  virtual double getPickedBoundingBoxMeta() const override;
  virtual bool boundingBoxPicked() const override;
  };
