#version 450 core

in vec3 vPos;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

out vec4 fragColor;

struct PointLight{
	vec3 pos;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

const int MAX_LIGHTS = 24;

layout(location = 17) uniform vec3 cameraPos;
layout(location = 18) uniform float zNear;
layout(location = 19) uniform float zFar;
layout(location = 20) uniform sampler2D positions;
layout(location = 21) uniform sampler2D normals;
layout(location = 22) uniform sampler2D albedo;
layout(location = 23) uniform sampler2D depth;
layout(location = 24) uniform int nrOfLights;
//layout(location = 25) uniform sampler2D octreeTest;
layout(location = 30) uniform PointLight pointLights[MAX_LIGHTS];

vec3 calcPointLight(PointLight light, vec3 pos, vec3 normal, vec4 objectColor){
	float distance = length(light.pos - pos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Diffuse
	vec3 lightDir = normalize(light.pos - pos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.color * diff * objectColor.rgb;

	// Specular
	vec3 viewDir = normalize(cameraPos - pos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = spec * objectColor.a * light.color;

	float fadeout = min(1, max(0, 2*2*2 - pow(length(light.pos - cameraPos) / 34, 2) + 1));

	return max(vec3(0, 0, 0), (specular + diffuse) * attenuation);
}

void main(){
	vec3 pos = texture(positions, vUV.xy).xyz;
	vec3 normal = texture(normals, vUV.xy).xyz;
	vec4 color = texture(albedo, vUV.xy);
	float z_b = texture(depth, vUV.xy).x;
	float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
	float test = (2.0 * zNear) / (zFar + zNear - z_b * (zFar - zNear));
	
	vec3 result = vec3(0);
	for(int i = 0; i < nrOfLights; i++){
		result += calcPointLight(pointLights[i], pos, normal, color);
	}
	result += color.xyz * 0.1f;

	fragColor = vec4(result, 0);
	gl_FragDepth = z_b;
}