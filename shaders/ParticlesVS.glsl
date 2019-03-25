#version 330

layout(location = 0) in vec4 inPos;
layout(location = 1) in float inFade;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);

uniform float inSize = 1;

out float fade;

void main(){
    fade = inFade;
    gl_PointSize = inSize;
    gl_Position = inPos * inVertToWorld * inWorldToCamera * inCameraToClip;
}
