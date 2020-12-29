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
		"\n" \
		"layout(std140) uniform Matrices {\n" \
		"	mat4 VP;\n" \
		"};\n" \
		"\n" \
		"out vec4 fColour;\n" \
		"out vec3 fNormal;\n" \
		"out vec3 fPosition;\n" \
		"\n" \
		"void main() {\n" \
		"	gl_Position = VP * vec4(inPosition, 1.0);\n" \
		"	fPosition = inPosition;\n" \
		"	fColour = inColour;\n" \
		"	// TODO(Abi): Normal matrix to undo translations and scaling\n" \
		"	fNormal = inNormal;\n" \
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
		"\n" \
		"out vec4 FragColour;\n" \
		"\n" \
		"const float AmbientStrength  = 1.0;\n" \
		"const float SpecularStrength = 0.5;\n" \
		"\n" \
		"void main() {\n" \
		"	vec3 Normal = normalize(fNormal);\n" \
		"	vec4 Ambient = AmbientStrength * SunColour;\n" \
		"	\n" \
		"	// NOTE(Abi): Diffuse Calculation\n" \
		"	float Diff = max(dot(Normal, SunDirection), 0.0);\n" \
		"	vec4 Diffuse = Diff * SunColour;\n" \
		"\n" \
		"	// NOTE(Abi): Specular Calculation\n" \
		"	vec3 ViewDirection = normalize(ViewPosition - fPosition);\n" \
		"	vec3 ReflectDir    = reflect(SunDirection, Normal);\n" \
		"	float Spec         = pow(max(dot(ViewDirection, ReflectDir), 0.0), 32);\n" \
		"	vec4 Specular      = Spec * SpecularStrength * SunColour;\n" \
		"\n" \
		"	vec4 Result = (Ambient + Diffuse + Specular) * fColour;\n" \
		"	FragColour  = vec4(Result.x, Result.y, Result.z, 1.0);\n" \
		"\n" \
		"    //if(gl_FragCoord.x < 400) FragColour = SunColour;\n" \
		"	\n" \
		"}\n"
	},
};