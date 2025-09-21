// phong_dir.vert
//
// Phong vertex shader for a single directional light source. All lighting
// calculations are performed in eye space. In eye space the origin is the
// camera's position. Performing all lighting in eye space means that we don't
// require the application to pass in the camera's position each time it uses
// this shader.

#version 400

// Precision qualifiers are added for code portability with OpenGL ES, not for
// functionality. According to the GLSL 1.30 and later specs: the same object
// declared in different shaders that are linked together must have the same
// precision qualification. This applies to inputs, outputs, uniforms, and
// globals.

//precision highp float;

// GLSL 1.30 (OpenGL 3.0) deprecates most of the built-in uniform state
// variables. In GLSL 1.40 (OpenGL 3.1) they have been removed. Typically we
// need to supply projection, view, and model matrices in order to transform
// vertices from model space to clip space.

uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat2 TextureMatrix;

// GLSL 1.30 (OpenGL 3.0) deprecates vertex shader attribute variables. They
// have been replaced with user-defined generic shader input variables. The
// application code uses the new glVertexAttribPointer() function to map vertex
// data stored in a vertex buffer object to these generic vertex shader input
// variables. This replaces the existing method of specifying vertex array data
// by calling glVertexPointer(), glNormalPointer(), etc.

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec4 VertexColorCoord;	// not used in this shader
layout (location = 3) in vec2 VertexTexCoord;	// not used inthis shader

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

out vec2 TexCoord;

// main vertex shader

void main()
{

	Normal = normalize(NormalMatrix * VertexNormal);							
	Eye    = vec3 (ModelViewMatrix * VertexPosition );
	Ldir   = DirectionalLight.cameraspacedirection.xyz;

   	gl_Position = ModelViewProjectionMatrix * VertexPosition;		 
	TexCoord    = TextureMatrix * VertexTexCoord;

	// material parameters

	Ambient=DirectionalLight.ambient*Material.ambient*DirectionalLight.power;
	Diffuse=DirectionalLight.diffuse*Material.diffuse*DirectionalLight.power;
	Specular=DirectionalLight.specular*Material.specular;
	Shininess=Material.shininess;
}


