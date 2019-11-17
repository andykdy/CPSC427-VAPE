#version 330

// From vertex shader
in vec2 vpos;

uniform vec3 color;

// Output color
layout(location = 0) out  vec4 out_color;

void main()
{

    out_color = vec4(color, 1.0);

    float radius = distance(vec2(0.0), vpos);

//    if (radius > 1.0)
//    {
        out_color.xyz += (1.0 - radius) * 0.6 * vec3(1.0f, 0.0f, 1.0f);
        //out_color.xyz -= vec3(0.f, 0.f, 0.f);
        //out_color.xyzw -= vec4(0.f, 0.f, 0.f, 0.6f);
        out_color.w = 0.5f;
//    }

}
