#version 450 core

layout(location = 0) in vec3 position;
out vec3 textureCoords;

layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 projectionMatrix;

void main(){
	textureCoords = position;
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0); 
	
	
}