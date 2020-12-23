@vertex
#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColour;

layout(std140) uniform Matrices {
	mat4 VP;
};

out vec4 fColour;

void main() {
	gl_Position = VP * vec4(inPosition, 1.0);
	fColour = inColour;
}

@fragment
#version 330 core

in vec4 fColour;

out vec4 FragColour;

void main() {
	FragColour = fColour;
}
