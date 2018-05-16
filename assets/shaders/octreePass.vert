#version 450 core

layout (location = 0) in vec3 pos;

out vec3 vPos;

layout(location = 1) uniform vec3 trans;
layout(location = 2) uniform mat4 scale;
layout(location = 6) uniform mat4 v;
layout(location = 7) uniform mat4 p;

void main(){
	vPos = trans + vec3(scale * vec4(pos, 1)).xyz;
	gl_Position = p * v * vec4(vPos, 1.0f);
}