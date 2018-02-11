#version 150

//-----FRAGMENT SHADER-----
out vec4 oColor;
in vec4 gColor;
in float timeStamp;

uniform float uPlayhead;

void main(void) {

	float timeDiff = 1000000.0f;

	//float pct = 1.0f - clamp(timeStamp - uPlayhead, 0.0f, timeDiff)/timeDiff;
	


	float length = 4405803.0f * 2.0f;
	float begin = 1503810000.0f;
	float pct = 1.0f - (timeStamp - uPlayhead)/timeDiff;

	oColor = gColor * vec4(1.0f, 1.0f, 1.0f, pct);

}