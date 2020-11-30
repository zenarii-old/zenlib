zen2d_shader_info ShaderInfo[] = {
	{
		"rectangles",
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) in vec2 inPosition;\n" \
		"layout(location = 1) in vec4 inColour;\n" \
		"\n" \
		"out vec4 fColour;\n" \
		"\n" \
		"void main() {\n" \
		"	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);\n" \
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
	{
		"lines",
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) in vec2 inPosition;\n" \
		"layout(location = 1) in vec4 inColour;\n" \
		"\n" \
		"out vec4 fColour;\n" \
		"\n" \
		"void main() {\n" \
		"	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);\n" \
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
	{
		"textures",
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) in vec2 inPosition;\n" \
		"layout(location = 1) in vec4 inColour;\n" \
		"layout(location = 2) in vec2 inTexCoords;\n" \
		"\n" \
		"out vec4 fColour;\n" \
		"out vec2 fTexCoords;\n" \
		"\n" \
		"void main() {\n" \
		"	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);\n" \
		"	fColour = inColour;\n" \
		"	fTexCoords = inTexCoords;\n" \
		"}\n" \
		"\n",
		"#version 330 core\n" \
		"\n" \
		"uniform sampler2D Texture;\n" \
		"in vec4 fColour;\n" \
		"in vec2 fTexCoords;\n" \
		"\n" \
		"out vec4 FragColour;\n" \
		"\n" \
		"void main() {\n" \
		"	FragColour = fColour * texture(Texture, fTexCoords);\n" \
		"}\n"
	},
	{
		"text",
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) in vec2 inPosition;\n" \
		"layout(location = 1) in vec4 inColour;\n" \
		"layout(location = 2) in vec2 inTexCoords;\n" \
		"\n" \
		"out vec4 fColour;\n" \
		"out vec2 fTexCoords;\n" \
		"\n" \
		"void main() {\n" \
		"	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);\n" \
		"	fColour = inColour;\n" \
		"	fTexCoords = inTexCoords;\n" \
		"}\n" \
		"\n",
		"#version 330 core\n" \
		"\n" \
		"uniform sampler2D Texture;\n" \
		"in vec4 fColour;\n" \
		"in vec2 fTexCoords;\n" \
		"\n" \
		"out vec4 FragColour;\n" \
		"//the smaller the text the higher the edge value desired\n" \
		"//TODO(Abi) Once i have a UI edit these values\n" \
		"const float Width = 0.32;\n" \
		"const float Edge = 0.34;\n" \
		"\n" \
		"void main() {\n" \
		"	float Distance = 1.0 - texture(Texture, fTexCoords).a;\n" \
		"	float Alpha = 1.0 - smoothstep(Width, Width + Edge, Distance);\n" \
		"\n" \
		"	FragColour = vec4(fColour.r, fColour.g, fColour.b, fColour.a * Alpha);\n" \
		"}\n"
	},
};