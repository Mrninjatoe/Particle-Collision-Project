#version 460 core

in vec4 gColor;
in vec2 gUV;

out vec4 fColor;

layout(location = 20) uniform sampler2D inTexture;

void main(){
	vec4 textureColor = texture(inTexture, vec2(gUV.x, 1 - gUV.y));
	fColor = gColor * textureColor;

	//gl_FragDepth = texture(depth, gUV).x;
}