#version 460 core

in vec4 gColor;
in vec2 gUV;

out vec4 fColor;

//layout(location = 20) uniform sampler2D depth;

void main(){
	fColor = gColor;
	//gl_FragDepth = texture(depth, gUV).x;
}