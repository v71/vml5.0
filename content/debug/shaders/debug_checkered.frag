// phong_dir.frag
//
// Phong fragment shader for a single directional light source. All lighting
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

precision highp float;

// Fragment shader input variable declarations must exactly match the vertex
// shader's output variable declarations. The output of the vertex shader and
// the input of the fragment shader form an interface. For this interface,
// vertex shader output variables and fragment shader input variables of the
// same name must match in type and qualification (other than out matching to in).

uniform uint chsize;
uniform vec4 col1;
uniform vec4 col2;
uniform vec3 CameraPosition;

in vec3 Ldir;
in vec3 Normal;
in vec3 Eye;

in vec4 Ambient;
in vec4 Diffuse;
in vec4 Specular;
in float Shininess;
in vec4 fragpos;
in vec2 TexCoord;

out vec4 outputColor;

void main()
{

    // set the specular term to black

    vec4 spec = vec4(0.0);

    // normalize input vectors
	
    vec3 l=normalize(Ldir);
    float intensity = max(dot(Normal,l), 0.0);
 
    // if the vertex is lit compute the specular color
    
	if (intensity > 0.0) 
	{
	    // compute the half vector
	    vec3 h = normalize(l + normalize(-Eye));  
    
	    // compute the specular term into spec
	    float intSpec = max(dot(h,Normal), 0.0);
	    spec = Specular * pow(intSpec,Shininess);
	}

 	float fmodResult = mod(floor(chsize * TexCoord.s) + floor(chsize * TexCoord.t), 2.0);
	
	vec4 col=col2;
	if (fmodResult < 1.0) col= col1;

	//  uncomment if you want a distance fog
	// *fogAmount must be set in place of outputColor.a;
	// outputColor.a=fogAmount;

	//	vec3 camFrag = fragpos.xyz-CameraPosition;
	//  float distance = length(camFrag);
	//  float b = 0.0006f;
	//	float fogAmount = exp(-distance*b);
	//	vec4 finalcolor=(col * intensity * Diffuse + spec + col * Ambient);
	//	finalcolor.a=fogAmount;
	//	outputColor=finalcolor;

	// another kind of fog

	//	float dist=800;
	//	float fogAmount = 1;
	//	if ( fragpos.z>dist)
	//	fogAmount=1-(fragpos.z-dist)/dist;
	
	outputColor = (col * intensity * Diffuse + spec + col * Ambient);
	outputColor.a=0.9f;
}

