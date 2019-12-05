#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform float falpha;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
    if (falpha != 0) {
        color = vec4(fcolor, falpha) * texture(sampler0, vec2(texcoord.x, texcoord.y));
    } else {
        color = vec4(fcolor, 1) * texture(sampler0, vec2(texcoord.x, texcoord.y));
    }
}
