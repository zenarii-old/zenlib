zen3d_shader_info ShaderInfo[] = {
	{
		"rgba",
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) in vec3 inPosition;\n" \
		"layout(location = 1) in vec4 inColour;\n" \
		"\n" \
		"layout(std140) uniform Matrices {\n" \
		"	mat4 VP;\n" \
		"};\n" \
		"\n" \
		"out vec4 fColour;\n" \
		"\n" \
		"void main() {\n" \
		"	gl_Position = VP * vec4(inPosition, 1.0);\n" \
		"	fColour = inColour;\n" \
		"}\n" \
		"\n",
		"#version 330 core\n" \
		"\n" \
		"in vec4 fColour;\n" \
		"\n" \
		"out vec4 FragColour;\n" \
		"\n" \
		"void main() {\n" \
		"	FragColour = fColour;\n" \
		"}\n"
	},
};