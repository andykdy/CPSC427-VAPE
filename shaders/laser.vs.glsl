#version 330

// Input attributes
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_translate;
layout (location = 2) in vec4 in_color;

uniform mat3 projection;

out vec2 vpos;
out vec4 vcolor;

void main()
{
    vpos = in_position.xy;
    vcolor = in_color;

    mat3 transform = mat3(
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    in_translate.x, in_translate.y, 1.0);

    vec3 pos = projection * transform * vec3(in_position.xy, 1.0);
    gl_Position = vec4(pos.xy, in_position.z, 1.0);
}