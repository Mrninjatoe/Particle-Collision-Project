#version 450 core

in vec3 textureCoords;
out vec4 out_Color;

layout(location = 10) uniform samplerCube cubeMap;

void main(){
    out_Color = texture(cubeMap, textureCoords);
	//out_Color = vec4(textureCoords, 1);
}