//
// Created by matt on 1/04/18.
//

#include <GameSystem/InputCodes.h>
#include "UIEditText.h"

UIEditText::UIEditText(uint id, const FontPtr& font) : UIText(id, font)
  {
  setMouseClickCallback([](uint x, uint y){ return true; });
  setTextPadding(4);
  setMultiLine(false);
  showCaret(false);
  setCaretPos(-1);
  characterLimit = std::numeric_limits<uint>::max();
  }

bool UIEditText::keyPress(GameContext* context, uint key)
  {
  if (key == KEY_BACKSPACE)
    backspaceAtCaret();
  else if (key == KEY_DELETE)
    deleteAtCaret();
  else if (key == KEY_SPACE && isSpaceAllowed())
    insertAtCaret(" ");
  else if (key == KEY_LEFT)
    moveCaretLeft();
  else if (key == KEY_RIGHT)
    moveCaretRight();
  else if (key == KEY_HOME)
    moveCaretToStart();
  else if (key == KEY_END)
    moveCaretToEnd();
  else if (key == KEY_ESC || key == KEY_ENTER)
    context->getUIManager()->lossFocus(context);
  else
    {
    char character = getCharFromKeyCode(key, context->getInputManager()->isKeyDown(KEY_LSHIFT));
    if (!verifyCharacterValid(character))
      return true;
    insertAtCaret(string() + character);
    }

  return true;
  }

void UIEditText::onGainFocus(GameContext* context)
  {
  showCaret(true);
  invalidate();
  }

void UIEditText::onLossFocus(GameContext* context)
  {
  showCaret(false);
  invalidate();
  }

void UIEditText::insertAtCaret(const string& text)
  {
  string currentText = getText();
  if (characterLimit - currentText.size() > 0)
    {
    int caret = getCaretPos();
    currentText.insert(caret, text, 0, characterLimit - currentText.size());
    setText(currentText);
    moveCaretRight((int) text.size());
    invalidate();
    }
  }

void UIEditText::backspaceAtCaret()
  {
  int caret = getCaretPos();
  if (caret > 0)
    {
    string text = getText();
    text.erase(caret-1, 1);
    moveCaretLeft();
    setText(text);
    invalidate();
    }
  }

void UIEditText::deleteAtCaret()
  {
  int caret = getCaretPos();
  string text = getText();
  if (caret < text.size())
    {
    text.erase(caret, 1);
    setText(text);
    invalidate();
    }
  }

void UIEditText::moveCaretLeft(int num)
  {
  moveCaret(-1 * num);
  }

void UIEditText::moveCaretRight(int num)
  {
  moveCaret(num);
  }

void UIEditText::moveCaret(int num)
  {
  int caret = getCaretPos() + num;
  caret = mathernogl::clampi(caret, 0, (int)getText().size());
  if (caret != getCaretPos())
    {
    setCaretPos(caret);
    invalidate();
    }
  }

void UIEditText::moveCaretToStart()
  {
  setCaretPos(0);
  invalidate();
  }

void UIEditText::moveCaretToEnd()
  {
  setCaretPos(-1);
  invalidate();
  }

bool UIEditText::verifyCharacterValid(char character) const
  {
  if (!isValidNonWhiteSpaceCharacter(character))
    return false;

  bool isNumber = character >= '0' && character <= '9';
  bool isAlphabet = character >= 'a' && character <= 'z' || character >= 'A' && character <= 'Z';

  switch (editType)
    {
    case typeAll:
      return true;
    case typeAlphaNumeric:
      return isNumber || isAlphabet;
    case typeUInt:
      return isNumber;
    case typeFloat:
      {
      if(isNumber)
        return true;
      if(character == '-')
        return getCaretPos() == 0 && getText().find('-') == std::string::npos;
      if(character == '.')
        return getCaretPos() != 0 && getText().find('.') == std::string::npos;
      return false;
      }
    }
  return false;
  }

bool UIEditText::isSpaceAllowed() const
  {
  return editType == typeAlphaNumeric || editType == typeAll;
  }

uint UIEditText::getTextUIntValue()
  {
  return (uint)std::max(stoi(getText()), 0);
  }

float UIEditText::getTextFloatValue()
  {
  return stof(getText());
  }
