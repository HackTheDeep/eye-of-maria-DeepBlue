#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in float uSize[];
in vec4 vColor[]; // Output from vertex shader for each vertex
out vec4 gColor; // Output to fragment shader
uniform mat4 ciProjectionMatrix;
uniform float uViewScale;

void main()
{
	gColor = vColor[0];
	float size = uSize[0];

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