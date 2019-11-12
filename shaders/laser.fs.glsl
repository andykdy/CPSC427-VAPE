#version 330

in vec2 vpos; // Distance from local origin
in vec4 vcolor;

uniform vec3 color;

// Output color
layout(location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(color * vcolor.xyz, vcolor.w);
}