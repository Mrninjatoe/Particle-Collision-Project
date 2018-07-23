#version 450 core

in vec4 gColor;
in vec2 gUV;

out vec4 fColor;

layout(location = 20) uniform sampler2D inTexture;
layout(location = 7) uniform mat4 p;
layout(location = 21) uniform vec3 forward;
void main(){
	vec4 textureColor = texture(inTexture, vec2(gUV.x, 1 - gUV.y));
	vec3 n;
	n.xy = gUV * 2.0 - vec2(1.0); 
	float r2 = dot(n.xy, n.xy);
	if(r2 > 1.0 - r2)
		discard;
	
	n.z = -sqrt(1.0 - r2);
	
	//vec4 pixelPos = vec4(gEyePos.xyz + n * gColor.a, 1);
	//vec4 clipSpacePos = p * pixelPos;
	//float depth = clipSpacePos.z / clipSpacePos.w;
	float diffuse = max(0.0, dot(n, vec3(0, 0.2f, -1)));
	float spec = max(pow(dot(n, normalize(forward + vec3(0, 0.2f, -1))), 250.f), 0);
	fColor = vec4(1) * diffuse + (spec * vec4(1) * vec4(1));
}