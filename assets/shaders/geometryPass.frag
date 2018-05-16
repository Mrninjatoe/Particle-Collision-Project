#version 450 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

layout (location = 22) uniform sampler2D modelTexture;

void main(){
	gPosition = vPos;
	gNormal = vNormal;
	//gAlbedoSpec = texture(modelTexture, vUV);
	//gAlbedoSpec = vec4(0.69, 0.39, 0.99, 0);
	gAlbedoSpec = vec4(vColor, 0);
}