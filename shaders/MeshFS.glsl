#version 330

#define DRAW_STYLE_SINGLE_COLOUR  1
#define DRAW_STYLE_TEXTURE        2
#define DRAW_STYLE_VERT_COLOUR    3

centroid in vec4 shadowMapPos;
centroid in vec3 normal;
centroid in vec3 colour;
centroid in vec2 texCoords;

out vec4 outputColour;

uniform int inLightShaded = 1;
uniform int inCellShadeGrades = 0;    // zero is no cell shading
uniform float inAlpha = 1;
uniform sampler2D inTexture;
uniform float inTextureColourMix = 0;
uniform int inDrawStyle = DRAW_STYLE_SINGLE_COLOUR;
uniform vec3 inLightDir = vec3(-0.5, -0.2, -0.3);
uniform vec3 inViewDir = vec3(0, 0, -1);
uniform sampler2DShadow inShadowMap;
uniform mat4 inShadowMapProjection;
uniform int inUseShadowMap = 0;
uniform vec2 inScreenSize;

vec3 getColour(){
  if (inDrawStyle == DRAW_STYLE_TEXTURE){
    vec3 texColour = texture(inTexture, texCoords).rgb;
    if (inTextureColourMix > 0)
      texColour = mix(texColour, colour, inTextureColourMix);
    return texColour;
  }
  else
    return colour;
}

float cellShadeClamp(float lightFactor)
  {
  int numCellShades = inCellShadeGrades;
  lightFactor *= numCellShades;
  lightFactor = round(lightFactor);
  lightFactor /= numCellShades;
  if (lightFactor == 0)
    lightFactor += (1.0 / numCellShades) * 0.1;
  return lightFactor;
  }

float gaussian[9] = float[]
  (
  0.077847,	0.123317,	0.077847,
  0.123317,	0.195346,	0.123317,
  0.077847,	0.123317,	0.077847
  );

float shadowLightFactor()
  {
  vec2 pixelSize = vec2(1.0, 1.0) / inScreenSize;
  if (inUseShadowMap < 1)
    return 1;
  float factor = 0;
  for (int kernelI = 0; kernelI < 3; ++kernelI)
    {
    for (int kernelJ = 0; kernelJ < 3; ++kernelJ)
      {
      vec4 adjustedCoord = shadowMapPos;
      adjustedCoord.x += pixelSize.x * (kernelI - 1);
      adjustedCoord.y += pixelSize.y * (kernelJ - 1);
      if (adjustedCoord.x > 0 && adjustedCoord.x < 1 && adjustedCoord.y > 0 && adjustedCoord.y < 1)
        factor += textureProj(inShadowMap, adjustedCoord) * gaussian[kernelI + kernelJ * 3];
      else
        factor += gaussian[kernelI + kernelJ * 3];
      }
    }
  return min(factor + 0.2, 1);
  }


void main()
  {
  if (inLightShaded > 0)
    {
    vec3 normalizedLight = normalize(inLightDir * -1);
    vec3 normalizedView = normalize(inViewDir * -1);

    float lightFactor = dot(normal, normalizedLight);
    lightFactor *= 0.5;
    lightFactor += 0.5;
    if (inCellShadeGrades > 0)
      lightFactor = cellShadeClamp(lightFactor);
    vec3 diffuseCol = getColour() * clamp(lightFactor, 0, 1);

//    vec3 halfwayVector = normalize((normalizedLight + normalizedView) * 0.5);
//    vec3 specularCol = vec3(1, 1, 1) * 0.1 * pow(clamp(dot(halfwayVector, normal), 0, 1), 6);

	  outputColour = vec4(diffuseCol * shadowLightFactor(), inAlpha);
    }
  else
    {
    outputColour = vec4(getColour() * shadowLightFactor(), inAlpha);
    }
  }
