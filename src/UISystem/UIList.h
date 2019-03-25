#pragma once

#include "UIPanel.h"
#include "UIButton.h"

/*
*   
*/

typedef std::pair<uint, string> IDAndText;
typedef std::function<void(uint id)> ItemSelectedFunc;

class UIList : public UIPanel
  {
private:
  mathernogl::MappedList<IDAndText> listItems;
  ItemSelectedFunc itemSelectedCallback;
  ItemSelectedFunc itemDblClickCallback;
  UIToggleButtonGroupPtr listItemsToggleGroup;
  std::list<uint> itemComponentIDs;
  int selectedItem = -1;
  uint itemHeight = 50;
  uint itemGap = 20;
  Vector3D itemColour = Vector3D(1);
  Vector3D itemSelectColour = Vector3D(0.1, 0.2, 0.8);
  Vector3D textColour;

public:
  UIList(uint id);

  virtual void initialise(GameContext* context) override;
  virtual void onUpdate(GameContext* context) override;
  virtual void cleanUp(GameContext* context) override;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;

  void addItem(uint id, string text);
  void removeItem(uint id);
  bool getSelectedItem(uint* id);

  void setItemSelectedCallback(ItemSelectedFunc func){ itemSelectedCallback = func; }
  void setItemDblClickedCallback(ItemSelectedFunc func){ itemDblClickCallback = func; }
  void setItemHeight(uint height) { itemHeight = height; }
  void setItemGap(uint gap) { itemGap = gap; }
  void setItemColour(const Vector3D& colour) { itemColour = colour; }
  void setItemSelectColour(const Vector3D& colour) { itemSelectColour = colour; }
  void setItemTextColour(const Vector3D& colour) { textColour = colour; }
  };
