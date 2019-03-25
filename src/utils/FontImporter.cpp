//
// Created by matt on 27/02/18.
//

#include <map>
#include "utils/FontImporter.h"
#include "utils/FileIO.h"

namespace mathernogl
  {

class BasicFontDefinition : public FontDefinition
  {
public:
  std::map<uint, FontCharacter> characters;
  uint lineHeight = 10;

  virtual bool containsCharacter(uint ascii) const override
    {
    return characters.count(ascii) > 0;
    }

  virtual const FontCharacter* getCharacterDefinition(uint ascii) const override
    {
    return &characters.at(ascii);
    }

  virtual uint getLineHeight() const override
    {
    return lineHeight;
    }
  };

struct Character
  {
  uint ascii;
  uint x;
  uint y;
  uint width;
  uint height;
  int xOffset;
  int yOffset;
  uint xAdvance;
  };

void parseFntFile(std::string fntPath, std::vector<Character>* parsedCharacters, uint* lineHeight, uint* base, uint* pageWidth, uint* pageHeight)
  {
  std::string fntFile = readTextFile(fntPath);

  std::stringstream sStream(fntFile);
  std::string word;

  Character character;
  bool uptoCharacterInfo = false;
  while (sStream >> word) {

    if (!uptoCharacterInfo && word.substr(0, word.find("=")) == "lineHeight") {
      *lineHeight = (uint)stoi(word.substr(word.find("=") + 1, 4));
      }

    if (!uptoCharacterInfo && word.substr(0, word.find("=")) == "base") {
      *base = (uint)stoi(word.substr(word.find("=") + 1, 4));
      }

    if (!uptoCharacterInfo && word.substr(0, word.find("=")) == "scaleW") {
      *pageWidth = (uint)stoi(word.substr(word.find("=") + 1, 4));
      }

    if (!uptoCharacterInfo && word.substr(0, word.find("=")) == "scaleH") {
      *pageHeight = (uint)stoi(word.substr(word.find("=") + 1, 4));
      }

    if (word == "chars") {
      sStream >> word;
      if (word.substr(0, word.find("=")) == "count") {
        sStream >> word;
        if (word == "char") {
          uptoCharacterInfo = true;
          }
        }
      }

    if (uptoCharacterInfo) {
      if (word.substr(0, word.find("=")) == "id") {
        character.ascii = (uint)stoi(word.substr(word.find("=") + 1, 4));
        }

      if (word.substr(0, word.find("=")) == "x") {
        character.x = (uint)stoi(word.substr(word.find("=") + 1, 4));
        }

      if (word.substr(0, word.find("=")) == "y") {
        character.y = (uint)stoi(word.substr(word.find("=") + 1, 4));
        }

      if (word.substr(0, word.find("=")) == "width") {
        character.width = (uint)stoi(word.substr(word.find("=") + 1, 4));
        }

      if (word.substr(0, word.find("=")) == "height") {
        character.height = (uint)stoi(word.substr(word.find("=") + 1, 4));
        }

      if (word.substr(0, word.find("=")) == "xoffset") {
        character.xOffset = stoi(word.substr(word.find("=") + 1, 4));
        }

      if (word.substr(0, word.find("=")) == "yoffset") {
        character.yOffset = stoi(word.substr(word.find("=") + 1, 4));
        }

      if (word.substr(0, word.find("=")) == "xadvance") {
        character.xAdvance = (uint)stoi(word.substr(word.find("=") + 1, 4));
        parsedCharacters->push_back(character);
        }

      if (word == "kernings") {
        uptoCharacterInfo = false;
        }
      }
    }
  }

FontDefinitionPtr readFontFile(const std::string& filePath)
  {
  BasicFontDefinition* fontDefinition = new BasicFontDefinition();

  std::vector<Character> characters;
  uint lineHeight = 0, base = 0, pageWidth = 0, pageHeight = 0;
  parseFntFile(filePath, &characters, &lineHeight, &base, &pageWidth, &pageHeight);

  for (Character character : characters)
    {
    FontCharacter fontChar;
    fontChar.ascii = character.ascii;
    fontChar.offset = Vector2D(character.xOffset, character.yOffset);
    fontChar.size = Vector2D(character.width, character.height);
    fontChar.cursorAdvance = character.xAdvance;
    fontChar.texCoordBL = Vector2D((float)character.x/(float)pageWidth, 1 - ((float)(character.y+character.height)/(float)pageHeight));
    fontChar.texCoordTR = Vector2D((float)(character.x+character.width)/(float)pageWidth, 1 - ((float)character.y/(float)pageHeight));
    fontDefinition->characters[fontChar.ascii] = fontChar;
    fontDefinition->lineHeight = lineHeight;
    }

  return FontDefinitionPtr(fontDefinition);
  }

  }