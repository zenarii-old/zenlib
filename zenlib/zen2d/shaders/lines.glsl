@vertex
#version 330 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColour;

out vec4 fColour;

void main() {
	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);
	fColour = inColour;
}

@fragment
#version 330 core

in vec4 fColour;

out vec4 FragColour;

void main() {
	FragColour = fColour;
}