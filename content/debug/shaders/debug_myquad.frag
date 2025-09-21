#version 410

// sets precision
//precision highp float;

uniform vec4 Color;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

layout(location = 0) out vec4 outputColor;
layout(location = 1) in vec3 nearPoint; // nearPoint calculated in vertex shader
layout(location = 2) in vec3 farPoint; // farPoint calculated in vertex shader


vec4 grid(vec3 fragPos3D, float scale) 
{
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0);
    if (line>1) color=vec4(0.1, 0.1, 0.2, 1.0);
    return color;
}

float computeDepth(vec3 pos) 
{
    vec4 clip_space_pos = ProjectionMatrix * ViewMatrix * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

void main()
{
    //	outputColor = vec4(0.1,0.0,0.0,1);
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = computeDepth(fragPos3D);
    if(t>0)
        outputColor = grid(fragPos3D, 0.01f);
}

