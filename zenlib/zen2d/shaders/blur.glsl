@vertex
#version 330 core

layout(location = 0) in vec2 inPosition;

out vec2 fTextureCoords;

void main() {
    gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);
    fTextureCoords = (inPosition+1.0)*0.5;
}

@fragment
#version 330 core

in vec2 fTextureCoords;
uniform sampler2D ScreenTexture;

out vec4 FragColour;

const float Offset = 1.0 / 300.0;  

void main()
{
    vec2 Offsets[9] = vec2[](
        vec2(-Offset,  Offset), // top-left
        vec2( 0.0f,    Offset), // top-center
        vec2( Offset,  Offset), // top-right
        vec2(-Offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( Offset,  0.0f),   // center-right
        vec2(-Offset, -Offset), // bottom-left
        vec2( 0.0f,   -Offset), // bottom-center
        vec2( Offset, -Offset)  // bottom-right    
    );

    float Kernel[9] = float[](
        1.0/16.0, 2.0/16.0, 1.0/16.0,
		2.0/16.0, 4.0/16.0, 2.0/16.0,
		1.0/16.0, 2.0/16.0, 1.0/16.0
    );
    
    vec3 SampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        SampleTex[i] = vec3(texture(ScreenTexture, fTextureCoords.st + Offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += SampleTex[i] * Kernel[i];
    
    FragColour = vec4(col, 1.0);
}  