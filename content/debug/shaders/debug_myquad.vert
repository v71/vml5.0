// simple single color debug shader

#version 410

// layout locations

layout (location = 0) in vec4 VertexPosition;
//layout (location = 1) in vec3 VertexNormal;		    // not used in this shader
//layout (location = 2) in vec4 VertexColorCoord;		// not used in this shader
//layout (location = 3) in vec2 VertexTexCoord;		// not used in this shader

// sets precision

//precision highp float;

// uniforms

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) 
{
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

layout(location = 1) out vec3 nearPoint;
layout(location = 2) out vec3 farPoint;

void main()
{

   // All vertex shaders should write the transformed homogeneous clip space
   // vertex position into the gl_Position variables.
    
   nearPoint = UnprojectPoint(VertexPosition.x, VertexPosition.y, 0.0, ViewMatrix, ProjectionMatrix).xyz; // unprojecting on the near plane
   farPoint =  UnprojectPoint(VertexPosition.x, VertexPosition.y, 1.0, ViewMatrix, ProjectionMatrix).xyz; // unprojecting on the far plane
   gl_Position = vec4(VertexPosition); // using directly the clipped coordinates
}
