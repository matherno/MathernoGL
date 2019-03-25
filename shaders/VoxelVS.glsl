#version 330

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inPosAndCol;

uniform float inVoxelSize = 1;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);
uniform mat4 inShadowMapProjection;
uniform int inUseShadowMap = 0;
uniform int inClippingEnabled = 0;
uniform vec4 inClipPlane;

centroid out vec4 shadowMapPos;
flat out vec3 normal;

flat out int colourIdx;

void performClipping(vec4 worldSpaceVert){
  gl_ClipDistance[0] = dot(worldSpaceVert, inClipPlane);
}

void main(){
    colourIdx = int(inPosAndCol.w);

    vec3 vert = inVertex + inPosAndCol.xyz;
    vert *= inVoxelSize;
    vert += inVoxelSize * 0.5;

    vec4 worldVertex = vec4(vert, 1) * inVertToWorld;

    if (inClippingEnabled > 0)
      performClipping(worldVertex);

    if (inUseShadowMap >= 1)
      shadowMapPos = worldVertex * inShadowMapProjection;

    gl_Position = worldVertex * inWorldToCamera * inCameraToClip;

    normal = normalize(inNormal * mat3(inVertToWorld));
}
