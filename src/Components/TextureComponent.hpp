//
// Created by Cody on 10/16/2019.
//

#include <iostream>
#include "MeshComponent.hpp"

#ifndef VAPE_TEXTURECOMPONENT_HPP
#define VAPE_TEXTURECOMPONENT_HPP

class TextureComponent : public ECS::Component {
private:
    Texture* texture;
    GLuint vao;
    GLuint vbo;
    GLuint ibo;

public:
    bool initTexture(Texture* texture) {
        this->texture = texture;

        // The position corresponds to the center of the texture
        float wr = texture->width * 0.5f;
        float hr = texture->height * 0.5f;

        TexturedVertex vertices[4];
        vertices[0].position = { -wr, +hr, -0.02f };
        vertices[0].texcoord = { 0.f, 1.f };
        vertices[1].position = { +wr, +hr, -0.02f };
        vertices[1].texcoord = { 1.f, 1.f };
        vertices[2].position = { +wr, -hr, -0.02f };
        vertices[2].texcoord = { 1.f, 0.f };
        vertices[3].position = { -wr, -hr, -0.02f };
        vertices[3].texcoord = { 0.f, 0.f };

        // counterclockwise as it's the default opengl front winding direction
        uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };

        // Clearing errors
        gl_flush_errors();

        // Vertex Buffer creation
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

        // Index Buffer creation
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

        // Vertex Array (Container for Vertex + Index buffer)
        glGenVertexArrays(1, &vao);

        return !gl_has_errors();
    }


    void draw(mat3 projection, mat3 transform, GLuint program) {
        // Setting shaders
        glUseProgram(program);

        // Enabling alpha channel for textures
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        // Getting uniform locations for glUniform* calls
        GLint transform_uloc = glGetUniformLocation(program, "transform");
        GLint color_uloc = glGetUniformLocation(program, "fcolor");
        GLint projection_uloc = glGetUniformLocation(program, "projection");

        // Setting vertices and indices
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        // Input data location as in the vertex buffer
        GLint in_position_loc = glGetAttribLocation(program, "in_position");
        GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
        glEnableVertexAttribArray(in_position_loc);
        glEnableVertexAttribArray(in_texcoord_loc);
        glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
        glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));

        // Enabling and binding texture to slot 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id);

        // Setting uniform values to the currently bound program
        glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
        float color[] = { 1.f, 1.f, 1.f };
        glUniform3fv(color_uloc, 1, color);
        glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

        // Drawing!
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    }

    void release() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
};

#endif //VAPE_TEXTURECOMPONENT_HPP
