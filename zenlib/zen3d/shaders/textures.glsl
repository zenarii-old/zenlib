@vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColour;
layout (location = 2) in vec2 inUVs;

layout(std140) uniform Matrices {
	mat4 VP;
};

out vec4 fColour;
out vec3 fPosition;
out vec2 fUVs;

void main() {
	gl_Position = VP * vec4(inPosition, 1.0);
	fPosition = inPosition;
	fColour = inColour;
	fUVs = inUVs;
}

@fragment
#version 330 core

in vec4 fColour;
in vec3 fPosition;
in vec2 fUVs;
uniform sampler2D Texture;

out vec4 FragColour;

void main() {
	FragColour  = texture(Texture, fUVs) * fColour;
}
