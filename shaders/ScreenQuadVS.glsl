#version 330

layout(location = 0) in vec2 inPos;

out vec2 texCoords;

void main(){
    vec2 inTexCoords = vec2(0, 0);
    if (inPos.x > 0)
      inTexCoords.x = 1;
    if (inPos.y > 0)
      inTexCoords.y = 1;
    texCoords = inTexCoords;
    gl_Position = vec4(inPos.x, inPos.y, 0, 1);
}
