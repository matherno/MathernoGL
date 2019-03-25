#version 330

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec3 inPos;
layout(location = 2) in float inLifeTimeFactor;   //  particle life span, 0 start -> 1 end
layout(location = 3) in float inTextureIndex;

uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);

uniform float inSize = 1;

out float lifeTimeFactor;
out vec2 texCoord;
flat out int texIndex;

void main()
  {
  texIndex = int(round(inTextureIndex));
  texIndex = clamp(texIndex, 0, 15);
  texCoord = vec2(0, 0);
  if (inVert.x > 0)
      texCoord.x = 1;
  if (inVert.y > 0)
      texCoord.y = 1;
  lifeTimeFactor = inLifeTimeFactor;
  vec3 rotatedVert = inVert * inverse(mat3(inWorldToCamera));
  gl_Position = vec4((rotatedVert * inSize) + inPos, 1) * inWorldToCamera * inCameraToClip;
  }
