#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColour;

uniform mat4 Projection;
uniform mat4 View;

out vec4 fColour;

void main() {
	gl_Position = Projection * View *  vec4(inPosition, 1.0);
	fColour = inColour;
}

