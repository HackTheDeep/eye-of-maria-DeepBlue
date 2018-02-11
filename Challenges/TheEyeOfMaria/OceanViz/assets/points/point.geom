#version 150

#define M_PI 3.1415926535897932384626433832795


layout(points) in;
layout(triangle_strip, max_vertices = 30) out;

in float uRadius[];
in vec4 vColor[]; // Output from vertex shader for each vertex
out vec4 gColor; // Output to fragment shader
uniform mat4 ciProjectionMatrix;
uniform float uViewScale;

vec4 getPointOnCircle( float rad, float ang){
	return rad * vec4( cos( M_PI * ang / 180.0f ), sin( M_PI * ang / 180.0f ), 0.0f, 0.0f);
}

void main()
{
	gColor = vColor[0];

	// int numPoints = 12;
	// float theta = 360.0f/float(numPoints);
	// for(int i = 0; i < numPoints; i++){
	// 	gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + getPointOnCircle( uRadius[0], i * theta ) );
	// 	gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + getPointOnCircle( uRadius[0] * 0.8f, i * theta ) );
	// 	EmitVertex();

	// }

	float size = uRadius[0];

	//make 4 corners around center to assemble triangle strip
	gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + vec4( -size / 2, -size / 2, 0.0f, 0.0f) * uViewScale);
	EmitVertex();
	gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + vec4(  size / 2, -size / 2, 0.0f, 0.0f) * uViewScale);
	EmitVertex();
	gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + vec4( -size / 2,  size / 2, 0.0f, 0.0f) * uViewScale);
	EmitVertex();
	gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + vec4(  size / 2,  size / 2, 0.0f, 0.0f) * uViewScale);
	EmitVertex();

	EndPrimitive();
}