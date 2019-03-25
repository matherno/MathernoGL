#version 330

uniform sampler2D inColourTexture;
uniform sampler2D inDepthTexture;
uniform vec2 inScreenSize;
uniform float inViewFrustumDepth = 1;
uniform mat4 inCameraToClip = mat4(1);

uniform float inViewNear = 1;
uniform float inViewFar = 2;

in vec2 texCoords;

out vec4 outputColour;

mat4 clipToCamera;

float getDepth(vec2 depthTexCoords)
  {
  float depth = texture(inDepthTexture, depthTexCoords).r;
  return (2.0 * depth - inViewNear - inViewFar) / (inViewFar - inViewNear);
  }

bool isEdgeDepth()
  {
  float pixelX = 1.0 / inScreenSize.x;
  float pixelY = 1.0 / inScreenSize.y;

  clipToCamera = inverse(inCameraToClip);
  float thisDepth = getDepth(texCoords);

  float avgAbsDepthDiff;
  avgAbsDepthDiff += abs( getDepth(texCoords + vec2(-pixelX, -pixelY))  - thisDepth );
  avgAbsDepthDiff += abs( getDepth(texCoords + vec2(pixelX, pixelY))  - thisDepth );
  avgAbsDepthDiff += abs( getDepth(texCoords + vec2(-pixelX, pixelY))  - thisDepth );
  avgAbsDepthDiff += abs( getDepth(texCoords + vec2(pixelX, -pixelY))  - thisDepth );
  avgAbsDepthDiff += abs( getDepth(texCoords + vec2(pixelX, 0))  - thisDepth );
  avgAbsDepthDiff += abs( getDepth(texCoords + vec2(0, pixelY))  - thisDepth );
  avgAbsDepthDiff += abs( getDepth(texCoords + vec2(-pixelX, 0)) - thisDepth );
  avgAbsDepthDiff += abs( getDepth(texCoords + vec2(0, -pixelY)) - thisDepth );
  avgAbsDepthDiff /= 8;

  return avgAbsDepthDiff > 0.0000005;
  }

float getLuma(vec4 colour)
  {
  return 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;
  }

bool isEdgeColour()
  {
  float pixelX = 1.0 / inScreenSize.x;
  float pixelY = 1.0 / inScreenSize.y;

  float pixel00 = getLuma(texture(inColourTexture, texCoords + vec2(-pixelX,  pixelY)));
  float pixel10 = getLuma(texture(inColourTexture, texCoords + vec2(0,        pixelY)));
  float pixel20 = getLuma(texture(inColourTexture, texCoords + vec2(pixelX,   pixelY)));

  float pixel01 = getLuma(texture(inColourTexture, texCoords + vec2(-pixelX,  0)));
  float pixel21 = getLuma(texture(inColourTexture, texCoords + vec2(pixelX,   0)));

  float pixel02 = getLuma(texture(inColourTexture, texCoords + vec2(-pixelX,  -pixelY)));
  float pixel12 = getLuma(texture(inColourTexture, texCoords + vec2(0,        -pixelY)));
  float pixel22 = getLuma(texture(inColourTexture, texCoords + vec2(pixelX,   -pixelY)));

  float sobelX = (pixel00 + 2 * pixel10 + pixel20) - (pixel02 + 2 * pixel12 + pixel22);
  float sobelY = (pixel00 + 2 * pixel01 + pixel02) - (pixel20 + 2 * pixel21 + pixel22);

  return (sobelX * sobelX + sobelY * sobelY) > 0.00001;
  }

void main()
  {
  vec4 renderColour = texture(inColourTexture, texCoords);

  if (isEdgeColour() && isEdgeDepth())
    outputColour = vec4(renderColour.xyz * 0.6, renderColour.a);
  else
    outputColour = renderColour;
  }