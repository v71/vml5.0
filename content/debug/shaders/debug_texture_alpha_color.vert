// simple texture debug shader


// note when importing textures
// black pixel background
// do not save color from transparent points( Gimp)
// do not salve background color ( Gimp )

#version 400

// layout locations

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;			// not used in this shader
layout (location = 2) in vec4 VertexColorCoord;		// not used in this shader
layout (location = 3) in vec2 VertexTexCoord;		

// sets precision

//precision highp float;

// uniforms

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

out vec2 TexCoord;
out vec4 ColorCoord;

// vertex shader

void main()
{
	// All vertex shaders should write the transformed homogeneous clip space
	// vertex position into the gl_Position variables.

	vec4 pos=vec4(VertexPosition.x,VertexPosition.y,VertexPosition.z,1);
	gl_Position = ModelViewProjectionMatrix*pos;
	TexCoord = VertexTexCoord;
	ColorCoord = VertexColorCoord;
	
}
