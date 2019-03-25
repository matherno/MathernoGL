//
// Created by matt on 27/02/18.
//

#include <set>
#include "UIText.h"

const static std::set<uint> whitespaceCharacters =
  {
    (uint)'\0',
    (uint)'\n',
    (uint)'\t',
    (uint)'\r',
    (uint)'\v',
    (uint)' ',
  };


UIText::UIText(uint id, FontPtr font) : UIPanel(id), font(font)
  {
  setColour(Vector3D(1));
  }

void UIText::initialise(GameContext* context)
  {
  if (!font)
    font = context->getDefaultFont();
  UIPanel::initialise(context);
  caretComponent.reset(new UIPanel(context->getUIManager()->getNextComponentID()));
  addChild(caretComponent);
  setVisible(true, false);
  }

void UIText::setText(string text)
  {
  this->text = text;
  }

void UIText::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  for (UIComponentPtr comp : characterComponents)
    removeChild(comp->getID());
  characterComponents.clear();
  UIPanel::refresh(context, parentPos, parentSize);

  ASSERT(font, "Don't have a font to use for this text!");
  if (font)
    {
    Vector2D cursor = Vector2D(0, 0);
    float textScaling = ((float)textSize * font->sizeScaling) / (float)font->fontDefinition->getLineHeight();
    buildText(context, &cursor, textScaling);
    caretComponent->setVisible(caretVisible, true);
    caretComponent->refresh(context, getCurrentScreenPos(), getCurrentScreenSize());
    }
  }

void UIText::buildCharacter(GameContext* context, FontCharPtr fontCharacter, Vector2D* cursor, float textScaling)
  {
  UIPanel* component = new UIPanel(context->getUIManager()->getNextComponentID());
  component->setHorizontalAlignment(alignmentStart);
  component->setVerticalAlignment(alignmentStart);
  component->setOffset(Vector2D(textPadding, 0) + *cursor + fontCharacter->offset * textScaling);
  component->setSize(fontCharacter->size * textScaling);
  component->setColour(fontColour);
  component->setTexture(font->fontGlyphsPage, true);
  component->setTextureCoords(fontCharacter->texCoordBL, fontCharacter->texCoordTR);
  component->setVisible(textVisible, false);
  component->setCanHitWithMouse(false);
  cursor->x += fontCharacter->cursorAdvance * textScaling;

  UIComponentPtr componentPtr(component);
  characterComponents.push_back(componentPtr);
  addChild(componentPtr);
  }

void UIText::buildWord(GameContext* context, const FontWord& fontWord, Vector2D* cursor, float textScaling)
  {
  for (FontCharPtr fontChar : fontWord)
    buildCharacter(context, fontChar, cursor, textScaling);
  }

void UIText::buildText(GameContext* context, Vector2D* cursor, float textScaling)
  {
  Vector2D textBounds = getCurrentScreenSize();
  textBounds.x -= textPadding * 2;
  const uint lineHeight = (uint)(font->fontDefinition->getLineHeight() * textScaling);

  uint spaceAdvance = 10;
  if (spaceWidth >= 0)
    spaceAdvance = (uint)spaceWidth;
  else if (font->fontDefinition->containsCharacter(' '))
    spaceAdvance = (uint)font->fontDefinition->getCharacterDefinition(' ')->cursorAdvance;
  spaceAdvance *= textScaling;

  FontWord fontWord;
  uint currentWordLength = 0;
  uint numLines = 1;
  uint maxLineLength = 0;
  uint numChars = 0;
  for (const char& character : text)
    {
    /*
     * if this is a non-whitespace character, add it to the word
     */
    uint ascii = (uint)character;
    if (isValidNonWhiteSpaceCharacter(ascii))
      {
      FontCharPtr fontChar = font->fontDefinition->getCharacterDefinition(ascii);
      fontWord.push_back(fontChar);

      currentWordLength += fontChar->cursorAdvance * textScaling;
      if (cursor->x + currentWordLength > textBounds.x && cursor->x != 0)
        {
        if (!multiLine)
          break;

        maxLineLength = (uint)std::max((float)maxLineLength, cursor->x);
        cursor->x = 0;
        cursor->y += lineHeight;
        ++numLines;
        }
      continue;
      }

    /*
     * must have reached whitespace, so build the word that we've been accumulating
     */
    if (cursor->y + lineHeight <= textBounds.y)
      {
      if (caretPos >= numChars && caretPos < numChars + fontWord.size())
        setupCaretInWord(*cursor, fontWord, caretPos - numChars, textScaling);
      buildWord(context, fontWord, cursor, textScaling);
      numChars += fontWord.size();
      fontWord.clear();
      currentWordLength = 0;
      }
    else
      {
      fontWord.clear();
      break;
      }

    /*
     * special whitespace characters to act upon
     */
    if (multiLine && character == '\n')
      {
      maxLineLength = (uint)std::max((float)maxLineLength, cursor->x);
      cursor->x = 0;
      cursor->y += lineHeight;
      ++numLines;
      }
    else if (character == ' ')
      {
      if (caretPos == numChars)
        setupCaret(*cursor);
      cursor->x += spaceAdvance;
      ++numChars;
      }
    }

  /*
   * if we have a word that's not built at this stage, make sure it's built
   */
  if (!fontWord.empty())
    {
    if (caretPos >= numChars && caretPos < numChars + fontWord.size())
      setupCaretInWord(*cursor, fontWord, caretPos - numChars, textScaling);
    buildWord(context, fontWord, cursor, textScaling);
    numChars += fontWord.size();
    }
  maxLineLength = (uint)std::max((float)maxLineLength, cursor->x);

  /*
   * put caret at the end of the text if required
   */
  if (caretPos >= numChars || caretPos < 0)
    setupCaret(*cursor);

  /*
   * do vertical alignment adjustment
   */
  if (centreAlignVert)
    {
    const uint paragraphHeight = numLines * lineHeight;
    const uint textShift = (uint)((textBounds.y - paragraphHeight) * 0.5f);
    for (UIComponentPtr comp : characterComponents)
      comp->setOffset(comp->getOffset() + Vector2D(0, textShift));
    caretComponent->setOffset(caretComponent->getOffset() + Vector2D(0, textShift));
    }

  /*
   * do horizontal alignment adjustment
   */
  if (centreAlignHoriz)
    {
    //  not the proper way of doing it, should be aligning line by line (works for single line text)
    const uint textShift = (uint)((textBounds.x - maxLineLength) * 0.5f);
    for (UIComponentPtr comp : characterComponents)
      comp->setOffset(comp->getOffset() + Vector2D(textShift, 0));
    caretComponent->setOffset(caretComponent->getOffset() + Vector2D(textShift, 0));
    }
  }

bool UIText::isValidNonWhiteSpaceCharacter(uint ascii) const
  {
  if (whitespaceCharacters.find(ascii) != whitespaceCharacters.end())
    return false;

  return font->fontDefinition->containsCharacter(ascii);
  }

void UIText::setVisible(bool visible, bool recurseChildren)
  {
  textVisible = visible;
  UIPanel::setVisible(visible, recurseChildren);
  if (visible)
    {
    UIPanel::setVisible(useBackground, false);
    if (caretComponent)
      caretComponent->setVisible(caretVisible, true);
    }
  }

void UIText::setupCaretInWord(const Vector2D& wordPosition, const FontWord& word, int wordCaretPos, float textScaling)
  {
  Vector2D caretCursorPos = wordPosition;
  for (int character = 0; character < wordCaretPos; ++character)
    caretCursorPos.x += word[character]->cursorAdvance * textScaling;
  setupCaret(caretCursorPos);
  }

void UIText::setupCaret(const Vector2D& cursorPos)
  {
  float sizeScaling = font ? font->sizeScaling : 1;
  caretComponent->setSize(Vector2D(2, textSize * sizeScaling));
  caretComponent->setOffset(Vector2D(textPadding, 0) + cursorPos);
  caretComponent->setColour(fontColour);
  caretComponent->setVisible(caretVisible, true);
  }

int UIText::getCaretPos() const
  {
  int pos = caretPos;
  if (pos < 0)
    pos = (int)text.size();
  return pos;
  }
