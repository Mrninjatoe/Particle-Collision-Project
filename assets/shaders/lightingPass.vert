#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

out vec3 vPos;
out vec2 vUV;

void main(){
	vPos = pos;
	vUV = uv;
	gl_Position = vec4(vPos, 1.0f);
}