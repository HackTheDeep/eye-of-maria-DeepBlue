#version 150

//-----FRAGMENT SHADER-----
out vec4 oColor;
in vec4 gColor;

void main(void) {
	oColor = gColor;
}