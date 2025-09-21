// simple texture fragment debug shader

#version 400

// sets precision

//precision highp float;

uniform sampler2D TextureSampler;

layout(location = 0) out vec4 outputColor;

in vec2 TexCoord;

void main()
{
	outputColor = texture(TextureSampler, TexCoord);
}

