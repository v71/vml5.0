// simple texture fragment debug shader

// note when importing textures
// black pixel background
// do not save color from transparent points( Gimp)
// do not salve background color ( Gimp )

#version 400

// sets precision

//precision highp float;

uniform sampler2D TextureSampler;
uniform float Alpha;

//smooth in vec4 ColorCoord;

in vec2 TexCoord;
in vec4 ColorCoord;

layout(location = 0) out vec4 outColor;

void main()
{

	vec4 texColor = texture(TextureSampler, TexCoord);

	if(texColor.a < 0.01)
		    discard;

	texColor *= ColorCoord;

//	outColor = vec4(texColor.rgb,Alpha );
//	outColor = vec4(texColor.rgb,texColor.a*Alpha );

	outColor = vec4( (texColor.rgb)*texColor.a*Alpha ,Alpha) ;

}

