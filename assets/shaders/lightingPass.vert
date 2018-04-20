#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uv;

out vec3 vPos;
out vec3 vNormal;
out vec3 vColor;
out vec2 vUV;

void main(){
	vPos = pos;
	vNormal = normal;
	vColor = color;
	vUV = uv;
	gl_Position = vec4(vPos, 1.0f);
}