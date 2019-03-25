#version 330

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoords;

uniform vec2 inScreenSize = vec2(800, 600);
uniform float inZDepth = 0;

out vec2 texCoords;

void main(){
    texCoords = inTexCoords;
    vec2 clipPos = inPos / inScreenSize;
    clipPos.y = 1.0 - clipPos.y;
    clipPos *= 2.0;
    clipPos -= 1.0;
    gl_Position = vec4(clipPos.x, clipPos.y, -1 * inZDepth, 1);
}
