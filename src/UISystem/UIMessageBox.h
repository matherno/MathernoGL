#pragma once

#include "UIPanel.h"

/*
*   
*/

class UIMessageBox : public UIPanel
  {
public:
  enum MsgBoxMode
    {
    modeOkay,
    modeContinueCancel,
    };

  enum MsgBoxResult
  {
  resultNone,
  resultCancel,
  resultContinue,
  };

  typedef std::function<void(UIMessageBox::MsgBoxResult result)> MsgBoxClosedCallback;

private:
  const string msg;
  const MsgBoxClosedCallback closeCallback;
  const MsgBoxMode mode;
  Vector3D borderColour = Vector3D(0.2);
  Vector3D interiorColour = Vector3D(0.3);
  Vector3D buttonColour = Vector3D(0.2);

public:
  UIMessageBox(uint id, const string& msg, MsgBoxMode mode, MsgBoxClosedCallback func);
  virtual void initialise(GameContext* context) override;
  virtual void onEscapePressed(GameContext* context) override;

  static void popupMessageBox(UIManager* uiManager, const string& msg, MsgBoxMode mode, MsgBoxClosedCallback closedCallback);
  };

