#version 330

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec4 inColour;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);

out vec4 colour;

void main(){
    colour = inColour;
    gl_Position = inPos * inVertToWorld * inWorldToCamera * inCameraToClip;
}
