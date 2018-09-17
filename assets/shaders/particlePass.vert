#version 450 core

layout (std430, binding = 0) buffer PositionBuffer {
	vec4 positions[];
};

layout (std430, binding = 1) buffer ColorBuffer {
	vec4 colors[];
};

out vec3 vPos;
out vec4 vColor;

void main(){
	vPos = positions[gl_InstanceID].xyz;
	vColor = colors[gl_InstanceID];
	gl_Position = vec4(vPos, 1);
}