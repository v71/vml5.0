#version 400

// sets precision

//precision highp float;

smooth in vec4 fragColor;

layout(location = 0) out vec4 outputColor;

void main()
{
	outputColor = fragColor;
}