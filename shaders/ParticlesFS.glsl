#version 330

uniform vec3 inColour;

in float fade;

out vec4 outputColour;

void main(){
	outputColour = vec4(inColour, 1.0 - (fade * fade));
}
