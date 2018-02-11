#version 150

//-----FRAGMENT SHADER-----
out vec4 oColor;
in vec4 gColor;
in float gTimeStamp;

uniform float uPlayhead;

void main(void) {

	// float timeDiff = 1000000.0f;
  //
	// //float pct = 1.0f - clamp(timeStamp - uPlayhead, 0.0f, timeDiff)/timeDiff;
  //
  //
  //
	// float length = 4405803.0f * 2.0f;
	// float begin = 1503810000.0f;
	// float pct = 1.0f - (timeStamp - uPlayhead)/timeDiff;

	// oColor = gColor * vec4(1.0f, 1.0f, 1.0f, pct);

	float trailDuration = 0.4; // out of 1.0
	float endTime = gTimeStamp + trailDuration;
	float progress = 1.0 - clamp((uPlayhead - gTimeStamp) / trailDuration, 0, 1.0);

	// since we have so many stacked samples, we need to work with lower alphas
	float alpha = pow(progress, 6.0);

	oColor = gColor * vec4(1.0, 1.0, 1.0, alpha);
}
