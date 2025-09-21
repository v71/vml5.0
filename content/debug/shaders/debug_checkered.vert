// simple texture debug shader

#version 400

// layout locations

layout (location = 0) in vec4 VertexPosition;
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
uniform mat2 TextureMatrix;

// Custom user defined structures used to model the directional light source
// and the characteristics of the material that the light will interact with.
// These are setup in the host application. This must match the declaration in
// the vertex shader.

uniform struct DirectionalLightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 cameraspacedirection;
	float power;
} DirectionalLight;

uniform struct MaterialStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} Material;

// GLSL 1.30 (OpenGL 3.0) also deprecates varying shader variables. This has
// been replaced with user-defined generic shader output variables. The output
// of the vertex shader and the input of the fragment shader form an interface.
// For this interface, vertex shader output variables and fragment shader input
// variables of the same name must match in type and qualification (other than
// out matching to in).

out vec3 Normal;
out vec3 Eye;
out vec3 Ldir;

out vec4 Ambient;
out vec4 Diffuse;
out vec4 Specular;
out float Shininess;
out vec4 fragpos;

// texcoord fragmetn

out vec2 TexCoord;

// vertex shader

void main()
{
	Normal = normalize(NormalMatrix * VertexNormal);							
	Eye = vec3 (ModelViewMatrix * VertexPosition );
	Ldir=DirectionalLight.cameraspacedirection.xyz;

	// All vertex shaders should write the transformed homogeneous clip space
	// vertex position into the gl_Position variables.

	fragpos = ModelViewProjectionMatrix*VertexPosition;
	gl_Position = fragpos;
	TexCoord = VertexTexCoord;

	// material parameters

	Ambient=DirectionalLight.ambient*Material.ambient*DirectionalLight.power;
	Diffuse=DirectionalLight.diffuse*Material.diffuse*DirectionalLight.power;
	Specular=DirectionalLight.specular*Material.specular;
	Shininess=Material.shininess;
}
