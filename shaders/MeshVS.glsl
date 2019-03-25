#version 330

#define DRAW_STYLE_SINGLE_COLOUR  1
#define DRAW_STYLE_TEXTURE        2
#define DRAW_STYLE_VERT_COLOUR    3

layout(location = 0) in vec4 inVert;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec3 inVertColour;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec3 inInstanceTransform1;
layout(location = 5) in vec3 inInstanceTransform2;
layout(location = 6) in vec3 inInstanceTransform3;
layout(location = 7) in vec3 inInstanceTransform4;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);
uniform vec3 inColour;
uniform int inDrawStyle = DRAW_STYLE_SINGLE_COLOUR;
uniform int inClippingEnabled = 0;
uniform vec4 inClipPlane;
uniform int inUseInstanceTransforms = 0;
uniform mat4 inShadowMapProjection;
uniform int inUseShadowMap = 0;

centroid out vec4 shadowMapPos;
centroid out vec3 normal;
centroid out vec3 colour;
centroid out vec2 texCoords;

void performClipping(vec4 worldSpaceVert){
  gl_ClipDistance[0] = dot(worldSpaceVert, inClipPlane);
}

void main(){
    mat4 vertToWorld = inVertToWorld;
    if (inUseInstanceTransforms >= 1) {
      vertToWorld = mat4(
        inInstanceTransform1.x, inInstanceTransform2.x, inInstanceTransform3.x, inInstanceTransform4.x,
        inInstanceTransform1.y, inInstanceTransform2.y, inInstanceTransform3.y, inInstanceTransform4.y,
        inInstanceTransform1.z, inInstanceTransform2.z, inInstanceTransform3.z, inInstanceTransform4.z,
        0, 0, 0, 1
      );
    }

    normal = normalize(inNorm * mat3(vertToWorld));
    colour = inColour;

    if (inDrawStyle == DRAW_STYLE_TEXTURE)
        texCoords = inTexCoord;
    else if (inDrawStyle == DRAW_STYLE_VERT_COLOUR)
        colour = inVertColour;

    vec4 worldVertex = inVert * vertToWorld;
    if (inClippingEnabled > 0)
      performClipping(worldVertex);

    if (inUseShadowMap >= 1)
      shadowMapPos = worldVertex * inShadowMapProjection;
    gl_Position = worldVertex * inWorldToCamera * inCameraToClip;
}
