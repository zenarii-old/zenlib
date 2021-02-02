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
	{
		"lighting",
		"#version 330 core\n" \
		"\n" \
		"layout (location = 0) in vec3 inPosition;\n" \
		"layout (location = 1) in vec4 inColour;\n" \
		"layout (location = 2) in vec3 inNormal;\n" \
		"layout (location = 3) in vec2 inUVs;\n" \
		"\n" \
		"\n" \
		"layout(std140) uniform Matrices {\n" \
		"	mat4 VP;\n" \
		"};\n" \
		"\n" \
		"out vec4 fColour;\n" \
		"out vec3 fNormal;\n" \
		"out vec3 fPosition;\n" \
		"out vec2 fUVs;\n" \
		"\n" \
		"void main() {\n" \
		"	gl_Position = VP * vec4(inPosition, 1.0);\n" \
		"	fPosition = inPosition;\n" \
		"	fColour = inColour;\n" \
		"	// TODO(Abi): Normal matrix to undo translations and scaling\n" \
		"	fNormal = inNormal;\n" \
		"	fUVs = inUVs;\n" \
		"}\n" \
		"\n",
		"#version 330 core\n" \
		"\n" \
		"layout(std140) uniform Lights {\n" \
		"	// Directional/sun\n" \
		"	vec3 SunDirection;\n" \
		"	vec4 SunColour;\n" \
		"\n" \
		"	vec3 ViewPosition;\n" \
		"};\n" \
		"\n" \
		"in vec4 fColour;\n" \
		"in vec3 fNormal;\n" \
		"in vec3 fPosition;\n" \
		"in vec2 fUVs;\n" \
		"uniform sampler2D Texture;\n" \
		"\n" \
		"out vec4 FragColour;\n" \
		"\n" \
		"const float AmbientStrength  = 0.3;\n" \
		"const float SpecularStrength = 0.3;\n" \
		"\n" \
		"void main() {\n" \
		"	vec3 nSunDirection = normalize(SunDirection);\n" \
		"\n" \
		"	vec3 Normal = normalize(fNormal);\n" \
		"	vec4 Ambient = AmbientStrength * SunColour;\n" \
		"	\n" \
		"	// NOTE(Abi): Diffuse Calculation\n" \
		"	float Diff = max(dot(Normal, -nSunDirection), 0.0);\n" \
		"	vec4 Diffuse = Diff * SunColour;\n" \
		"\n" \
		"	// NOTE(Abi): Specular Calculation\n" \
		"	vec3 ViewDirection = normalize(ViewPosition - fPosition);\n" \
		"	vec3 ReflectDir    = reflect(nSunDirection, Normal);\n" \
		"	float Spec         = pow(max(dot(ViewDirection, ReflectDir), 0.0), 8);\n" \
		"	vec4 Specular      = Spec * SpecularStrength * SunColour;\n" \
		"\n" \
		"	vec4 Result = (Ambient + Diffuse + Specular) * fColour;\n" \
		"	Result.a = 1.0;\n" \
		"	FragColour  = Result * texture(Texture, fUVs);\n" \
		"\n" \
		"    //if(gl_FragCoord.x < 400) FragColour = SunColour;\n" \
		"	\n" \
		"}\n"
	},
	{
		"textures",
		"#version 330 core\n" \
		"\n" \
		"layout (location = 0) in vec3 inPosition;\n" \
		"layout (location = 1) in vec4 inColour;\n" \
		"layout (location = 2) in vec2 inUVs;\n" \
		"\n" \
		"layout(std140) uniform Matrices {\n" \
		"	mat4 VP;\n" \
		"};\n" \
		"\n" \
		"out vec4 fColour;\n" \
		"out vec3 fPosition;\n" \
		"out vec2 fUVs;\n" \
		"\n" \
		"void main() {\n" \
		"	gl_Position = VP * vec4(inPosition, 1.0);\n" \
		"	fPosition = inPosition;\n" \
		"	fColour = inColour;\n" \
		"	fUVs = inUVs;\n" \
		"}\n" \
		"\n",
		"#version 330 core\n" \
		"\n" \
		"in vec4 fColour;\n" \
		"in vec3 fPosition;\n" \
		"in vec2 fUVs;\n" \
		"uniform sampler2D Texture;\n" \
		"\n" \
		"out vec4 FragColour;\n" \
		"\n" \
		"void main() {\n" \
		"	FragColour  = texture(Texture, fUVs) * fColour;\n" \
		"	FragColour.a = 1.0;\n" \
		"}\n"
	},
};