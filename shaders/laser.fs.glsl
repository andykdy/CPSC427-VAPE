#version 330

in vec3 vcolor;
in vec2 vpos; // Distance from local origin

uniform float laser_center;
uniform vec color;

// Output color
layout(location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(fcolor * vcolor, 1.0);
    // TODO adjust color depending on distance from laser_center (x value)
}