// simple single color debug shader

#version 400

// layout locations

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;		// not used in this shader
layout (location = 2) in vec4 VertexColorCoord;		// not used in this shader
layout (location = 3) in vec2 VertexTexCoord;		// not used in this shader

// sets precision

//precision highp float;

// uniforms

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat2 TextureMatrix;

// vertex shader

void main()
{

   // All vertex shaders should write the transformed homogeneous clip space
   // vertex position into the gl_Position variables.
    
    gl_Position = ModelViewProjectionMatrix * VertexPosition;
    
}
