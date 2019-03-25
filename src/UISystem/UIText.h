#pragma once

#include "UIPanel.h"

/*
*   
*/

class UIText : public UIPanel
  {
private:
  std::vector<UIComponentPtr> characterComponents;
  FontPtr font;
  string text;
  uint textSize = 25;
  Vector3D fontColour;
  int spaceWidth = -1;   // -1 => use fonts space width definition
  bool useBackground = true;
  bool textVisible = true;
  bool centreAlignVert = false;
  bool centreAlignHoriz = false;
  uint textPadding = 0;
  bool multiLine = true;

  std::shared_ptr<UIPanel> caretComponent;
  int caretPos = -1;    //  position in text to insert the caret (-1 means at the end)
  bool caretVisible = false;

public:
  UIText(uint id, FontPtr font = nullptr);      // will use default font if one not given
  void setText(string text);
  string getText() const { return text; }
  void setFontSize(uint textSize) { this->textSize = std::max(textSize, 1u); }
  uint getFontSize() const { return textSize; }
  void setFontColour (const Vector3D& colour) { fontColour = colour; }
  Vector3D getFontColour() const { return fontColour; }
  void setSpaceWidth (uint width) { spaceWidth = width; }
  void setTextCentreAligned(bool vertical, bool horizontal) { centreAlignVert = vertical; centreAlignHoriz = horizontal; }
  void showBackground(bool show) { useBackground = show; }
  void setTextPadding(uint padding) { textPadding = padding; }
  void setMultiLine(bool multiLine) { this->multiLine = multiLine; }
  void showCaret(bool show) { caretVisible = show; }
  void setCaretPos(int position) { caretPos = position; }
  int getCaretPos() const;

  virtual void initialise(GameContext* context) override;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;
  virtual void setVisible(bool visible, bool recurseChildren) override;

protected:
  typedef const mathernogl::FontCharacter* FontCharPtr;
  typedef std::vector<FontCharPtr> FontWord;
  void buildCharacter(GameContext* context, FontCharPtr fontCharacter, Vector2D* cursor, float textScaling);
  void buildWord(GameContext* context, const FontWord& fontWord, Vector2D* cursor, float textScaling);
  void buildText(GameContext* context, Vector2D* cursor, float textScaling);
  bool isValidNonWhiteSpaceCharacter(uint ascii) const;
  void setupCaretInWord(const Vector2D& wordPosition, const FontWord& word, int wordCaretPos, float textScaling);
  void setupCaret(const Vector2D& cursorPos);
  };
