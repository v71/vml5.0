#version 400

// sets precision

//precision highp float;

uniform vec4 Color;

layout(location = 0) out vec4 outputColor;

void main()
{
    
	outputColor = Color;

}

