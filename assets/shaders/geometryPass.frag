#version 460 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

//out vec4 fColor;

layout (location = 22) uniform sampler2D modelTexture;

void main(){
	gPosition = vPos;
	gNormal = vNormal;
	gAlbedoSpec = texture(modelTexture, vec2(vUV.x, 1 - vUV.y));
	//gAlbedoSpec = vec4(1,0,0,0);
	//fColor = vec4(vPos, 0);
}