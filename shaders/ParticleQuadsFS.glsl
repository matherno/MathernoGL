#version 330

uniform vec3 inColour = vec3(1, 0, 1);
uniform float inColourMixFactor = 2;
uniform sampler2D inTexSamplers[16];
uniform int inNumTexMapCellsX = 1;
uniform int inNumTexMapCellsY = 1;

in float lifeTimeFactor;
in vec2 texCoord;
flat in int texIndex;

out vec4 outputColour;

vec4 textureAt(int index, vec2 texCoord);

vec4 getTextureMapCell(int textureSeq)
  {
  int col = textureSeq % inNumTexMapCellsX;
  int row = textureSeq / inNumTexMapCellsX;
  row = inNumTexMapCellsY - row - 1;

  vec2 cellSize = vec2(1, 1) / vec2(inNumTexMapCellsX, inNumTexMapCellsY);
  vec2 cellTexCoord = texCoord * cellSize;
  cellTexCoord += cellSize * vec2(col, row);
  return textureAt(texIndex, cellTexCoord);
  }

vec4 getTextureMapColour()
  {
  int numTexMapCells = inNumTexMapCellsX * inNumTexMapCellsY;
  float textureSeq = clamp(lifeTimeFactor * (numTexMapCells - 1), 0, numTexMapCells - 1);
  int firstTexture = int(floor(textureSeq));
  int secondTexture = int(ceil(textureSeq));
  float textureMix = textureSeq - firstTexture;
  return mix(getTextureMapCell(firstTexture), getTextureMapCell(secondTexture), textureMix);
  }

void main()
  {
  float alpha = 1.0 - (lifeTimeFactor * lifeTimeFactor);

  if (inColourMixFactor >= 1)
    {
    outputColour = vec4(inColour, alpha);
    }
  else
    {
    vec4 textureColour;
    textureColour = getTextureMapColour();
    textureColour.a *= alpha;

    if (inColourMixFactor > 0)
      outputColour = vec4(mix(textureColour.rgb, inColour, inColourMixFactor), textureColour.a);
    else
      outputColour = textureColour;
    }
  }

vec4 textureAt(int index, vec2 texCoord)
  {
  switch (index)
    {
    case 0: return texture(inTexSamplers[0], texCoord);
    case 1: return texture(inTexSamplers[1], texCoord);
    case 2: return texture(inTexSamplers[2], texCoord);
    case 3: return texture(inTexSamplers[3], texCoord);
    case 4: return texture(inTexSamplers[4], texCoord);
    case 5: return texture(inTexSamplers[5], texCoord);
    case 6: return texture(inTexSamplers[6], texCoord);
    case 7: return texture(inTexSamplers[7], texCoord);
    case 8: return texture(inTexSamplers[8], texCoord);
    case 9: return texture(inTexSamplers[9], texCoord);
    case 10: return texture(inTexSamplers[10], texCoord);
    case 11: return texture(inTexSamplers[11], texCoord);
    case 12: return texture(inTexSamplers[12], texCoord);
    case 13: return texture(inTexSamplers[13], texCoord);
    case 14: return texture(inTexSamplers[14], texCoord);
    case 15: return texture(inTexSamplers[15], texCoord);
    }
  return vec4(0, 0, 0, 1);
  }
