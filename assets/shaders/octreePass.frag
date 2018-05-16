#version 450 core

in vec3 vPos;

out vec4 fColor;

layout(location = 9) uniform vec4 color;

void main(){
	fColor = color;
}