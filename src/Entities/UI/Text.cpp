//
// Created by Cody on 11/20/2019.
//

#include "Text.hpp"

bool Text::init(Font *font) {
    // Loading shaders
    if (!effect.load_from_file(shader_path("font.vs.glsl"), shader_path("font.fs.glsl")))
        throw std::runtime_error("Failed to load font textured shader");

    m_font = font;
    setText("");
    m_color = {1.f,1.f,1.f};
    m_alpha = 1.f;
    return true;
}

void Text::clear() {
    if (mesh.vbo != 0) {
        glDeleteBuffers(1, &mesh.vbo);
        mesh.vbo = 0;
    }
    if (mesh.ibo != 0) {
        glDeleteBuffers(1, &mesh.ibo);
        mesh.ibo = 0;
    }
    if (mesh.vao != 0){
        glDeleteVertexArrays(1, &mesh.vao);
    }
    vertices.clear();
    indices.clear();
}

void Text::destroy() {
    clear();
    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Text::draw(const mat3 &projection) {
    transform.begin();
    transform.translate(motion.position);
    transform.scale({1,1});
    transform.rotate(0);
    transform.end();

    // Setting shaders
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint alpha_uloc = glGetUniformLocation(effect.program, "falpha");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_texcoord_loc = glGetAttribLocation(effect.program, "in_texcoord");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_texcoord_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_font->getTexture());

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);
    float color[] = { m_color.x, m_color.y, m_color.z };
    glUniform3fv(color_uloc, 1, color);
    glUniform1f(alpha_uloc, m_alpha);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, nullptr);
}

void Text::setText(const char *text, float z) {
    clear();

    float x = 0;
    float y = 0;

    uint16_t idx = 0;
    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(m_font->getCdata(), 512,512, *text-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9
            vertices.emplace_back(TexturedVertex{{q.x0, q.y1, z}, {q.s0, q.t1}});
            vertices.emplace_back(TexturedVertex{{q.x0, q.y0, z}, {q.s0, q.t0}});
            vertices.emplace_back(TexturedVertex{{q.x1, q.y0, z}, {q.s1, q.t0}});
            vertices.emplace_back(TexturedVertex{{q.x1, q.y1, z}, {q.s1, q.t1}});
            indices.emplace_back(idx);
            indices.emplace_back(idx+1);
            indices.emplace_back(idx+2);
            indices.emplace_back(idx);
            indices.emplace_back(idx+2);
            indices.emplace_back(idx+3);
            idx += 4;
        }
        ++text;
    }

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        throw std::runtime_error("Failed to set text");
}
