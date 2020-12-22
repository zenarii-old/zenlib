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
	{
		"framebuffer_blit",
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) in vec2 inPosition;\n" \
		"layout(location = 1) in vec2 inTextureCoords;\n" \
		"\n" \
		"out vec2 fTextureCoords;\n" \
		"\n" \
		"void main() {\n" \
		"    gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);\n" \
		"    fTextureCoords = inTextureCoords;\n" \
		"}\n" \
		"\n",
		"#version 330 core\n" \
		"\n" \
		"in vec2 fTextureCoords;\n" \
		"uniform sampler2D ScreenTexture;\n" \
		"\n" \
		"out vec4 FragColour;\n" \
		"\n" \
		"void main() {\n" \
		"	/*\n" \
		"	if(texture(ScreenTexture, fTextureCoords).a < 0.1) {\n" \
		"		FragColour = vec4(1.0);\n" \
		"	}\n" \
		"	else if(texture(ScreenTexture, fTextureCoords).a > 0.9) {\n" \
		"		FragColour = vec4(1.0, 0.0, 0.0, 1.0);\n" \
		"	}*/\n" \
		"\n" \
		"	\n" \
		"    FragColour = texture(ScreenTexture, fTextureCoords);\n" \
		"	\n" \
		"}\n"
	},
	{
		"blur",
		"#version 330 core\n" \
		"\n" \
		"layout(location = 0) in vec2 inPosition;\n" \
		"\n" \
		"out vec2 fTextureCoords;\n" \
		"\n" \
		"void main() {\n" \
		"    gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);\n" \
		"    fTextureCoords = (inPosition+1.0)*0.5;\n" \
		"}\n" \
		"\n",
		"#version 330 core\n" \
		"\n" \
		"in vec2 fTextureCoords;\n" \
		"uniform sampler2D ScreenTexture;\n" \
		"\n" \
		"out vec4 FragColour;\n" \
		"\n" \
		"const float Offset = 1.0 / 300.0;  \n" \
		"\n" \
		"void main()\n" \
		"{\n" \
		"    vec2 Offsets[9] = vec2[](\n" \
		"        vec2(-Offset,  Offset), // top-left\n" \
		"        vec2( 0.0f,    Offset), // top-center\n" \
		"        vec2( Offset,  Offset), // top-right\n" \
		"        vec2(-Offset,  0.0f),   // center-left\n" \
		"        vec2( 0.0f,    0.0f),   // center-center\n" \
		"        vec2( Offset,  0.0f),   // center-right\n" \
		"        vec2(-Offset, -Offset), // bottom-left\n" \
		"        vec2( 0.0f,   -Offset), // bottom-center\n" \
		"        vec2( Offset, -Offset)  // bottom-right    \n" \
		"    );\n" \
		"\n" \
		"    float Kernel[9] = float[](\n" \
		"        1.0/16.0, 2.0/16.0, 1.0/16.0,\n" \
		"		2.0/16.0, 4.0/16.0, 2.0/16.0,\n" \
		"		1.0/16.0, 2.0/16.0, 1.0/16.0\n" \
		"    );\n" \
		"    \n" \
		"    vec3 SampleTex[9];\n" \
		"    for(int i = 0; i < 9; i++)\n" \
		"    {\n" \
		"        SampleTex[i] = vec3(texture(ScreenTexture, fTextureCoords.st + Offsets[i]));\n" \
		"    }\n" \
		"    vec3 col = vec3(0.0);\n" \
		"    for(int i = 0; i < 9; i++)\n" \
		"        col += SampleTex[i] * Kernel[i];\n" \
		"    \n" \
		"    FragColour = vec4(col, 1.0);\n" \
		"}  \n"
	},
};