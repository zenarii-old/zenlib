@vertex
#version 330 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColour;
layout(location = 2) in vec2 inTexCoords;

out vec4 fColour;
out vec2 fTexCoords;

void main() {
	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);
	fColour = inColour;
	fTexCoords = inTexCoords;
}

@fragment
#version 330 core

uniform sampler2D Texture;
in vec4 fColour;
in vec2 fTexCoords;

out vec4 FragColour;

void main() {
	FragColour = fColour * texture(Texture, fTexCoords);
}