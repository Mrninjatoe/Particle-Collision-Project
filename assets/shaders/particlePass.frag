#version 450 core

in vec4 gColor;
in vec2 gUV;

out vec4 fColor;

layout(location = 7) uniform mat4 p;
layout(location = 21) uniform vec3 forward;
//layout(location = 22) uniform sampler2D particleTexture;
void main(){
	vec3 n;
	n.xy = gUV * 2.0 - vec2(1.0); 
	float r2 = dot(n.xy, n.xy);
	if(r2 > 1.0 - r2)
		discard;
	
	n.z = -sqrt(1.0 - r2);

	//vec4 pTexture = texture(particleTexture, vec2(gUV.x, 1- gUV.y));
	float diffuse = max(0.0, dot(n, vec3(0, 0.2f, -1)));
	float spec = max(pow(dot(n, normalize(forward + vec3(0, 0.2f, -1))), 250.f), 0);
	fColor = gColor * diffuse + (spec * vec4(1));
}