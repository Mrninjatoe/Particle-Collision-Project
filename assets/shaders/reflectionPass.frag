#version 450 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

layout(location = 0) out vec3 gRnormal;
layout(location = 22) uniform sampler2D diffuseTexture; // UGLY FIX, SHOULD MAKE A NEW FUNCTION IN GLRENDERER FOR REFLECTED PASS.

void main(){
	gRnormal = vNormal; 
}