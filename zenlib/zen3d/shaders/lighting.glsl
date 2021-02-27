@vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColour;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inUVs;


layout(std140) uniform Matrices {
	mat4 VP;
};

out vec4 fColour;
out vec3 fNormal;
out vec3 fPosition;
out vec2 fUVs;

void main() {
	gl_Position = VP * vec4(inPosition, 1.0);
	fPosition = inPosition;
	fColour = inColour;
	// TODO(Abi): Normal matrix to undo translations and scaling
	fNormal = inNormal;
	fUVs = inUVs;
}

@fragment
#version 330 core

layout(std140) uniform Lights {
	// Directional/sun
	vec3 SunDirection;
	vec4 SunColour;

	vec3 ViewPosition;
};

in vec4 fColour;
in vec3 fNormal;
in vec3 fPosition;
in vec2 fUVs;
uniform sampler2D Texture;

out vec4 FragColour;

const float AmbientStrength  = 0.3;
const float SpecularStrength = 0.3;

void main() {
	vec3 nSunDirection = normalize(SunDirection);

	vec3 Normal = normalize(fNormal);
	vec4 Ambient = AmbientStrength * SunColour;
	
	// NOTE(Abi): Diffuse Calculation
	float Diff = max(dot(Normal, -nSunDirection), 0.0);
	vec4 Diffuse = Diff * SunColour;

	// NOTE(Abi): Specular Calculation
	vec3 ViewDirection = normalize(ViewPosition - fPosition);
	vec3 ReflectDir    = reflect(nSunDirection, Normal);
	float Spec         = pow(max(dot(ViewDirection, ReflectDir), 0.0), 8);
	vec4 Specular      = Spec * SpecularStrength * SunColour;

	vec4 Result = (Ambient + Diffuse + Specular) * fColour;
	Result.a = 1.0;
	FragColour = Result * texture(Texture, fUVs);
	FragColour = v4(1.f, 1.f, 1.f, 1.f);
    //if(gl_FragCoord.x < 400) FragColour = SunColour;
	
}
