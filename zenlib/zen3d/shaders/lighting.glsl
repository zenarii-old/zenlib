@vertex
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColour;
layout (location = 2) in vec3 inNormal;

layout(std140) uniform Matrices {
	mat4 VP;
};

out vec4 fColour;
out vec3 fNormal;
out vec3 fPosition;

void main() {
	gl_Position = VP * vec4(inPosition, 1.0);
	fPosition = inPosition;
	fColour = inColour;
	// TODO(Abi): Normal matrix to undo translations and scaling
	fNormal = inNormal;
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

out vec4 FragColour;

const float AmbientStrength  = 1.0;
const float SpecularStrength = 0.5;

void main() {
	vec3 Normal = normalize(fNormal);
	vec4 Ambient = AmbientStrength * SunColour;
	
	// NOTE(Abi): Diffuse Calculation
	float Diff = max(dot(Normal, SunDirection), 0.0);
	vec4 Diffuse = Diff * SunColour;

	// NOTE(Abi): Specular Calculation
	vec3 ViewDirection = normalize(ViewPosition - fPosition);
	vec3 ReflectDir    = reflect(SunDirection, Normal);
	float Spec         = pow(max(dot(ViewDirection, ReflectDir), 0.0), 32);
	vec4 Specular      = Spec * SpecularStrength * SunColour;

	vec4 Result = (Ambient + Diffuse + Specular) * fColour;
	FragColour  = vec4(Result.x, Result.y, Result.z, 1.0);

    //if(gl_FragCoord.x < 400) FragColour = SunColour;
	
}
