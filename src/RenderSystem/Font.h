#pragma once

#include "utils/FontImporter.h"
#include "texture/Texture.h"

/*
*   
*/

struct Font
  {
  const mathernogl::FontDefinitionPtr fontDefinition;
  std::shared_ptr<mathernogl::Texture> fontGlyphsPage;
  const float sizeScaling = 1;

  Font(mathernogl::FontDefinitionPtr& fontDefinition, std::shared_ptr<mathernogl::Texture>& fontGlyphsPage, float scaling)
    : fontDefinition(fontDefinition), fontGlyphsPage(fontGlyphsPage), sizeScaling(scaling) {}
  };

typedef std::shared_ptr<Font> FontPtr;
