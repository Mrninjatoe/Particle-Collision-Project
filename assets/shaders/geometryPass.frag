#version 460 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

out vec4 fragColor;

layout (location = 22) uniform sampler2D modelTexture;

void main(){
	fragColor = texture(modelTexture, vec2(vUV.x, 1 - vUV.y));
}