#version 150

uniform mat4 ciModelView;
uniform float uPlayhead;
in vec4 ciPosition;
in vec4 ciColor;
in float iPointRadius;
in float iTimeStamp;
out float uRadius;
out vec4 vColor;
out float vTimeStamp;

void main(void) {
	gl_Position = ciModelView * ciPosition;
	uRadius = iPointRadius;
	vTimeStamp = iTimeStamp;

	if (vTimeStamp <= uPlayhead) {
		vColor = ciColor;
	} else {
		vColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}
