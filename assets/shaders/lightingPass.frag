#version 460 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

out vec4 fragColor;

layout(location = 20) uniform sampler2D positions;
layout(location = 21) uniform sampler2D normals;
layout(location = 22) uniform sampler2D albedo;
layout(location = 23) uniform sampler2D depth;

void main(){
	vec3 pos = texture(positions, vUV.xy).xyz;
	vec3 normal = texture(normals, vUV.xy).xyz;
	vec4 color = texture(albedo, vUV.xy);
	float d = texture(depth, vUV.xy).r;

	fragColor = color * d;
}