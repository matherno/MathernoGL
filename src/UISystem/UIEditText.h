#pragma once

#include "UIText.h"

/*
*   Just supports single line text properly at the moment
*/

class UIEditText : public UIText
  {
public:
  enum EditType
    {
    typeAll,
    typeAlphaNumeric,   //  a-z, A-Z, 0-9
    typeUInt,
    typeFloat,
    };

private:
  uint characterLimit;
  EditType editType = typeAll;

public:
  UIEditText(uint id, const FontPtr& font = nullptr);
  virtual bool keyPress(GameContext* context, uint key) override;
  virtual void onGainFocus(GameContext* context) override;
  virtual void onLossFocus(GameContext* context) override;
  void setCharacterLimit(uint limit) { characterLimit = limit; }
  void setEditType(EditType type) { editType = type; }
  uint getTextUIntValue();
  float getTextFloatValue();

protected:
  void moveCaretLeft(int num = 1);
  void moveCaretRight(int num = 1);
  void moveCaret(int num);
  void insertAtCaret(const string& text);
  void backspaceAtCaret();
  void deleteAtCaret();
  void moveCaretToStart();
  void moveCaretToEnd();
  bool verifyCharacterValid(char character) const;
  bool isSpaceAllowed() const;
  };
