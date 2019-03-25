#version 330

#define DRAW_STYLE_SINGLE_COLOUR  1
#define DRAW_STYLE_TEXTURE        2
#define DRAW_STYLE_ALPHA_TEXTURE  3     // uses single colour for RGB, and texture for alpha

uniform vec3 inColour = vec3(0, 0, 0.5);
uniform int inDrawStyle = DRAW_STYLE_SINGLE_COLOUR;
uniform sampler2D inTexture;

in vec2 texCoords;

out vec4 outputColour;

void main(){
  if (inDrawStyle == DRAW_STYLE_SINGLE_COLOUR)
    outputColour = vec4(inColour, 1);
	else if (inDrawStyle == DRAW_STYLE_TEXTURE)
	  outputColour = texture(inTexture, texCoords);
	else if (inDrawStyle == DRAW_STYLE_ALPHA_TEXTURE)
	  outputColour = vec4(inColour, texture(inTexture, texCoords).a);
	else
	  outputColour = vec4(0.5, 0, 0.5, 1);
}
