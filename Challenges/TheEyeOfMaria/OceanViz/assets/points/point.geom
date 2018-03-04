#version 150

#define M_PI 3.1415926535897932384626433832795


layout(points) in;
layout(triangle_strip, max_vertices = 60) out;

in float uRadius[];
in vec4 vColor[]; // Output from vertex shader for each vertex
out vec4 gColor; // Output to fragment shader
uniform mat4 ciProjectionMatrix;
// uniform float uViewScale;
uniform float uPlayhead;
in float vTimeStamp[];
out float gTimeStamp;

vec4 getPointOnCircle( float rad, float ang){
	return rad * vec4( cos( M_PI * ang / 180.0f ), sin( M_PI * ang / 180.0f ), 0.0f, 0.0f);
}

void main()
{
	gColor = vColor[0];
	gTimeStamp = vTimeStamp[0];
	float size = uRadius[0];

	int numPoints = 20;
	float theta = 360.0f/float(numPoints);
	for(int i = 0; i < numPoints + 1; i++){
		gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + getPointOnCircle( size, 			i * theta ) );
		EmitVertex();
		gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + getPointOnCircle( size * 0.5f, 	i * theta ) );
		EmitVertex();

	}


	// //make 4 corners around center to assemble triangle strip
	// gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + vec4( -size / 2, -size / 2, 0.0f, 0.0f) * uViewScale);
	// EmitVertex();
	// gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + vec4(  size / 2, -size / 2, 0.0f, 0.0f) * uViewScale);
	// EmitVertex();
	// gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + vec4( -size / 2,  size / 2, 0.0f, 0.0f) * uViewScale);
	// EmitVertex();
	// gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + vec4(  size / 2,  size / 2, 0.0f, 0.0f) * uViewScale);
	// EmitVertex();

	EndPrimitive();
}
