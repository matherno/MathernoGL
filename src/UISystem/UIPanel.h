#pragma once

#include "UISystem.h"
#include "UIRenderable.h"

/*
*   
*/

typedef std::function<void(const Vector2D& parentPos, const Vector2D& parentSize)> OnRefreshCallback;

class UIPanel : public UIComponent
  {
private:
  std::shared_ptr<UIRenderable> renderable;
  Vector2D offset;
  Vector2D size;
  bool widthMatchParent = false;
  bool heightMatchParent = false;
  float horizPadding = 0;
  float vertPadding = 0;
  Alignment horizAlignment = alignmentStart;
  Alignment vertAlignment = alignmentStart;
  Vector3D colour;
  TexturePtr texture;
  bool alphaTexture = false;
  Vector2D texCoordBL = Vector2D(0, 0);
  Vector2D texCoordTR = Vector2D(1, 1);
  mathernogl::MappedList<UIComponentPtr> children;
  std::list<UIComponentPtr> childrenToAdd;
  std::list<uint> childrenToRemove;
  Vector2D currentScreenPos, currentScreenSize;
  bool visible = true;
  OnRefreshCallback onRefreshCallback;
  bool canHitWithMouse = true;

protected:
  OnMouseClickCallback mouseClickCallback;
  OnMouseClickCallback mouseDblClickCallback;
  bool isValid = false;

public:
  UIPanel(uint id);

  virtual void initialise(GameContext* context) override;
  virtual void onUpdate(GameContext* context) override;
  virtual void cleanUp(GameContext* context) override;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;
  virtual bool needsRefresh() override;
  virtual void invalidate() override;
  virtual void addChild(UIComponentPtr component) override;
  virtual void removeChild(uint id) override;
  virtual UIComponentPtr getChild(uint id, bool recurseChildren = false) override;
  virtual uint mouseClick(GameContext* context, uint mouseX, uint mouseY) override;
  virtual uint mouseDblClick(GameContext* context, uint mouseX, uint mouseY) override;
  virtual bool keyPress(GameContext* context, uint key) override { return false; }
  virtual void onGainFocus(GameContext* context) override {}
  virtual void onLossFocus(GameContext* context) override {}
  virtual uint hitTest(uint mouseX, uint mouseY, bool testChildren = false) override;
  virtual void onEscapePressed(GameContext* context) override {};
  virtual void onMouseEnter() override {};
  virtual void onMouseExit() override {};
  virtual void setCanHitWithMouse(bool canHit) override { canHitWithMouse = canHit; }

  virtual void setColour(const Vector3D& colour) { this->colour = colour; }
  Vector3D getColour() const { return colour; }
  virtual void setTexture(TexturePtr texture, bool alphaTexture = false) { this->texture = texture; this->alphaTexture = alphaTexture; }
  virtual void setTextureCoords(const Vector2D& bottomLeft, const Vector2D& topRight) { texCoordBL = bottomLeft; texCoordTR = topRight; }
  virtual void setOffset(const Vector2D& offset) override { this->offset = offset; }
  virtual void setSize(const Vector2D& size) override { this->size = size; }
  virtual Vector2D getOffset() const override { return offset; }
  virtual Vector2D getSize() const override { return size; }
  virtual void setWidthMatchParent(bool match) override { widthMatchParent = match; }
  virtual void setHeightMatchParent(bool match) override { heightMatchParent = match; }
  virtual void setHorizontalAlignment(Alignment alignment) override { horizAlignment = alignment; }
  virtual void setVerticalAlignment(Alignment alignment) override { vertAlignment = alignment; }
  virtual void setVisible(bool visible, bool recurseChildren = false) override;
  virtual bool isVisible() const override { return visible; }
  virtual void setMouseClickCallback(OnMouseClickCallback func) override { mouseClickCallback = func; }
  virtual void setMouseDblClickCallback(OnMouseClickCallback func) override { mouseDblClickCallback = func; }
  virtual void setPadding(float horizPadding, float vertPadding) override;
  virtual void setRenderDepth(float* depth, float depthIncrement) override;

  void setOnRefreshCallback(OnRefreshCallback func) { onRefreshCallback = func; }
  Vector2D getCurrentScreenPos() const { return currentScreenPos; }
  Vector2D getCurrentScreenSize() const { return currentScreenSize; }

private:
  void addPendingComponents(GameContext* context);
  void removePendingComponents(GameContext* context);
  void refreshPendingComponents(GameContext* context);
  };
