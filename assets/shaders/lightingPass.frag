#version 450 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

out vec4 fragColor;

layout(location = 18) uniform float zNear;
layout(location = 19) uniform float zFar;
layout(location = 20) uniform sampler2D positions;
layout(location = 21) uniform sampler2D normals;
layout(location = 22) uniform sampler2D albedo;
layout(location = 23) uniform sampler2D depth;

void main(){
	vec3 pos = texture(positions, vUV.xy).xyz;
	vec3 normal = texture(normals, vUV.xy).xyz;
	vec4 color = texture(albedo, vUV.xy);
	float z_b = texture(depth, vUV.xy).x;
	float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
	float test = (2.0 * zNear) / (zFar + zNear - z_b * (zFar - zNear));
   

	//fragColor = vec4(1) * z_b;
	//fragColor = vec4(normal, 1) * z_e;
	fragColor = color;
	gl_FragDepth = z_b;
	//fragColor = vec4(1) * z_e;
}