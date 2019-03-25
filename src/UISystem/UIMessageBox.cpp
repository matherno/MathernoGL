//
// Created by matt on 3/04/18.
//

#include "UIMessageBox.h"
#include "UIButton.h"

UIMessageBox::UIMessageBox(uint id, const string& msg, MsgBoxMode mode, MsgBoxClosedCallback func) : UIPanel(id), msg(msg), closeCallback(func), mode(mode)
  {}

void UIMessageBox::initialise(GameContext* context)
  {
  UIManager* uiManager = context->getUIManager();
  setHorizontalAlignment(alignmentCentre);
  setVerticalAlignment(alignmentCentre);
  setSize(Vector2D(400, 90));
  setColour(borderColour);

  UIPanel::initialise(context);

  UIPanel* interiorComponent = new UIPanel(uiManager->getNextComponentID());
  interiorComponent->setHeightMatchParent(true);
  interiorComponent->setWidthMatchParent(true);
  interiorComponent->setPadding(3, 3);
  interiorComponent->setColour(interiorColour);
  addChild(UIComponentPtr(interiorComponent));

  UIText* msgComponent = new UIText(uiManager->getNextComponentID());
  msgComponent->setText(msg);
  msgComponent->setTextCentreAligned(false, true);
  msgComponent->setFontSize(30);
  msgComponent->setSize(Vector2D(0, 30));
  msgComponent->setWidthMatchParent(true);
  msgComponent->setColour(interiorColour);
  msgComponent->setOffset(Vector2D(0, 7));
  msgComponent->setPadding(5, 5);
  interiorComponent->addChild(UIComponentPtr(msgComponent));

  float offsetX = -7;
  auto addButton = [this, interiorComponent, uiManager, &offsetX](string text, MsgBoxResult result)
    {
    UIButton* button = new UIButton(uiManager->getNextComponentID(), false);
    button->setSize(Vector2D(120, 35));
    button->setOffset(Vector2D(offsetX, -7));
    button->setVerticalAlignment(alignmentEnd);
    button->setHorizontalAlignment(alignmentEnd);
    button->setButtonText(text, Vector3D(0), Vector3D(0.05, 0.1, 0.4), 25);
    button->setButtonColour(buttonColour);
    button->setButtonHighlightColour(Vector3D(0.2, 0.2, 0.3));
    button->setMouseClickCallback([this, result](uint mx, uint my)
        {
        closeCallback(result);
        return true;
        });
    interiorComponent->addChild(UIComponentPtr(button));
    offsetX -= 130;
    };

  switch(mode)
    {
    case modeOkay:
      addButton("Okay", resultNone);
      break;
    case modeContinueCancel:
      addButton("Cancel", resultCancel);
      addButton("Continue", resultContinue);
      break;
    }
  }

void UIMessageBox::onEscapePressed(GameContext* context)
  {
  //  default results when escape is pressed....
  switch(mode)
    {
    case modeOkay:
      closeCallback(resultNone);
      break;
    case modeContinueCancel:
      closeCallback(resultCancel);
      break;
    }
  }

void UIMessageBox::popupMessageBox(UIManager* uiManager, const string& msg, MsgBoxMode mode, MsgBoxClosedCallback closedCallback)
  {
  uint msgBoxID = uiManager->getNextComponentID();
  MsgBoxClosedCallback superClosedCallback = [uiManager, msgBoxID, closedCallback](MsgBoxResult result)
    {
    uiManager->removeComponent(msgBoxID);
    uiManager->popModalComponent();
    if (closedCallback)
      closedCallback(result);
    };

  std::shared_ptr<UIMessageBox> msgBox(new UIMessageBox(msgBoxID, msg, mode, superClosedCallback));
  uiManager->addComponent(msgBox);
  uiManager->pushModalComponent(msgBox);
  }
