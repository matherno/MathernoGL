#pragma once

/*
*   
*/

#include <mathernogl/maths/Vector3D.h>
#include <memory>

namespace mathernogl
  {

  /*
   *  offset => offset of the bottom left of the glyph from the cursor
   *  size => size of the quad to render the glyph on
   *  cursorAdvance => horizontal advance of the cursor after rendering this glyph
   *  texCoordBL => texture coordinate of the bottom left corner of the quad
   *  texCoordTR => texture coordinate of the top right corner of the quad
   */
struct FontCharacter
  {
  uint ascii;
  Vector2D offset;
  Vector2D size;
  int cursorAdvance;
  Vector2D texCoordBL;
  Vector2D texCoordTR;
  };

class FontDefinition
  {
public:
  virtual bool containsCharacter(uint ascii) const = 0;
  virtual const FontCharacter* getCharacterDefinition(uint ascii) const = 0;
  virtual uint getLineHeight() const = 0;
  };
typedef std::shared_ptr<FontDefinition> FontDefinitionPtr;

FontDefinitionPtr readFontFile(const std::string& filePath);

  }
