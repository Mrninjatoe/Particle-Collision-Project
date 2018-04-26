#version 460 core

layout (std430, binding = 0) buffer PositionBuffer {
		vec4 positions[];
};

layout (std430, binding = 1) buffer DirectionBuffer {
		vec4 directions[];
};

layout (std430, binding = 2) buffer ColorBuffer {
	vec4 colors[];
};

layout(std430, binding = 3) buffer VelocityBuffer{
	vec4 velocities[];
};

out vec3 vPos;
out vec3 vDir;
out vec4 vColor;

void main(){
	vPos = positions[gl_InstanceID].xyz;
	vDir = directions[gl_InstanceID].xyz;
	vColor = colors[gl_InstanceID];
	gl_Position = vec4(vPos, 1);
}