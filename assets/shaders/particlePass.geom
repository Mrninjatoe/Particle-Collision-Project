#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 vPos[];
in vec3 vDir[];
in vec4 vColor[];

out vec4 gColor;
out vec2 gUV;

layout(location = 6) uniform mat4 v;
layout(location = 7) uniform mat4 p;

void main(void){
	vec3 right = vec3(
		v[0][0],
		v[1][0],
		v[2][0]
	);

	vec3 up = vec3(
		v[0][1],
		v[1][1],
		v[2][1]
	);
	
	vec3 point = vPos[0];

	// Vertex A of the billboarded quad.
	vec3 vertA = point - (right + up) * vColor[0].a;
	//gPos = vertA;
	gl_Position = p * v * vec4(vertA, 1.0);
	gUV = vec2(0.0, 0.0);
	gColor = vColor[0];
	EmitVertex();

	vec3 vertB = point - (right - up) * vColor[0].a;
	//gPos = vertB;
	gl_Position = p * v * vec4(vertB, 1.0);
	gUV = vec2(0.0, 1.0);
	gColor = vColor[0];
	EmitVertex();

	vec3 vertD = point + (right - up) * vColor[0].a;
	//gPos = vertD;
	gl_Position = p * v * vec4(vertD, 1.0);
	gUV = vec2(1.0, 0.0);	
	gColor = vColor[0];
	EmitVertex();

	vec3 vertC = point + (right + up) * vColor[0].a;
	//gPos = vertC;
	gl_Position = p * v * vec4(vertC, 1.0);
	gUV = vec2(1.0, 1.0);
	gColor = vColor[0];
	EmitVertex();

	EndPrimitive();
}