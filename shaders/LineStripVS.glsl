#version 330

layout(location = 0) in vec4 inPos;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);

void main(){
    gl_Position = inPos * inVertToWorld * inWorldToCamera * inCameraToClip;
}
