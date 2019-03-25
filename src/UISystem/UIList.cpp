//
// Created by matt on 30/03/18.
//

#include "UIList.h"

UIList::UIList(uint id) : UIPanel(id)
  {
  listItemsToggleGroup.reset(new UIToggleButtonGroup());
  }

void UIList::initialise(GameContext* context)
  {
  UIPanel::initialise(context);
  }

void UIList::onUpdate(GameContext* context)
  {
  UIPanel::onUpdate(context);
  }

void UIList::cleanUp(GameContext* context)
  {
  UIPanel::cleanUp(context);
  }

void UIList::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  for (uint id : itemComponentIDs)
    removeChild(id);
  itemComponentIDs.clear();
  selectedItem = -1;

  uint buttonOffset = 0;
  UIManager* uiManager = context->getUIManager();
  std::shared_ptr<UIButton> listItemButton;
  for (IDAndText idAndText : *listItems.getList())
    {
    listItemButton.reset(new UIButton(uiManager->getNextComponentID(), true));
    listItemButton->setGroup(listItemsToggleGroup);
    listItemButton->setButtonText(idAndText.second, textColour, itemSelectColour, itemHeight*0.7);
    listItemButton->setButtonHighlightColour(itemSelectColour, itemColour);
    listItemButton->setSize(Vector2D(0, itemHeight));
    listItemButton->setOffset(Vector2D(0, buttonOffset));
    listItemButton->setWidthMatchParent(true);
    listItemButton->setButtonColour(itemColour);
    listItemButton->setHighlightWidth(3);
    listItemButton->setMouseClickCallback([this, idAndText, listItemButton](uint mouseX, uint mouseY)
          {
          if (listItemButton->isToggledDown())
            {
            selectedItem = idAndText.first;
            if (itemSelectedCallback)
              itemSelectedCallback(idAndText.first);
            }
          else if (selectedItem == idAndText.first)
            selectedItem = -1;
          return true;
          });
    listItemButton->setMouseDblClickCallback([this, idAndText](uint mouseX, uint mouseY)
          {
          selectedItem = idAndText.first;
          if (itemDblClickCallback)
            itemDblClickCallback(idAndText.first);
          return true;
          });
    addChild(listItemButton);
    itemComponentIDs.push_back(listItemButton->getID());
    buttonOffset += itemHeight + itemGap;
    }

  UIPanel::refresh(context, parentPos, parentSize);
  }

void UIList::addItem(uint id, string text)
  {
  listItems.add(std::make_pair(id, text), id);
  }

void UIList::removeItem(uint id)
  {
  listItems.remove(id);
  }

bool UIList::getSelectedItem(uint* id)
  {
  if (selectedItem < 0)
    return false;
  *id = (uint)selectedItem;
  return true;
  }
