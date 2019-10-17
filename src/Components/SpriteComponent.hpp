//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_SPRITECOMPONENT_HPP
#define VAPE_SPRITECOMPONENT_HPP

#include <Engine/ECS/Component.hpp>
#include <iostream>

class SpriteComponent : public ECS::Component {
private:
    Texture* texture;
    int totalSprites;
    int index;
    GLuint vao;
    GLuint vertexDataBuffer;
    GLuint* indexBuffers;

public:
    bool initTexture(Texture* texture) {
        return initTexture(texture, 1, texture->width, texture->height);
    }

    bool initTexture(Texture* texture, int totalSprites, int spriteW, int spriteH) {
        this->texture = texture;
        index = 0;
        this->totalSprites = totalSprites;

        // Clearing errors
        gl_flush_errors();

        // Allocate Vertex Data Buffer
        TexturedVertex vertexData [ totalSprites * 4 ];
        glGenBuffers(1, &vertexDataBuffer);

        // Allocate Index Buffers
        indexBuffers = new GLuint[ totalSprites ];
        glGenBuffers(totalSprites, indexBuffers);

        // uint16_t indices[ 4 ] = { 0, 0, 0, 0 };
        uint16_t indices[ 6 ] = { 0, 0, 0, 0, 0, 0 };
        float wr = spriteW * 0.5f;
        float hr = spriteH * 0.5f;
        for (int i = 0; i < totalSprites; ++i ) {
            // Assume only horizontal spritesheets for now
            int xcurs = i*spriteW;
            float clipL = (float)xcurs / texture->width;
            float clipR = (float)(xcurs+spriteW) / texture->width;

            int n = i * 4;

            vertexData[n].position = { -wr, +hr, -0.02f };
            vertexData[n].texcoord = { clipL, 1.f };

            vertexData[n+1].position = { +wr, +hr, -0.02f };
            vertexData[n+1].texcoord = { clipR, 1.f };

            vertexData[n+2].position = { +wr, -hr, -0.02f };
            vertexData[n+2].texcoord = { clipR, 0.f };

            vertexData[n+3].position = { -wr, -hr, -0.02f };
            vertexData[n+3].texcoord = { clipL, 0.f };

            indices[ 0 ] = static_cast<uint16_t>(i * 4 + 0);
            indices[ 1 ] = static_cast<uint16_t>(i * 4 + 3);
            indices[ 2 ] = static_cast<uint16_t>(i * 4 + 1);
            indices[ 3 ] = static_cast<uint16_t>(i * 4 + 1);
            indices[ 4 ] = static_cast<uint16_t>(i * 4 + 3);
            indices[ 5 ] = static_cast<uint16_t>(i * 4 + 2);

            // Bind sprite index buffer data
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[ i ]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);
        }
        // Bind vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vertexDataBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * totalSprites * 4, vertexData, GL_STATIC_DRAW);

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
        glBindBuffer(GL_ARRAY_BUFFER, vertexDataBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[ index ]);

        // Input data location as in the vertex buffer
        GLint in_position_loc = glGetAttribLocation(program, "in_position");
        GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
        glEnableVertexAttribArray(in_position_loc);
        glEnableVertexAttribArray(in_texcoord_loc);
        glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (GLvoid*) offsetof( TexturedVertex, position ) );
        glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (GLvoid*) offsetof( TexturedVertex, texcoord ) );

        // Enabling and binding texture to slot 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id);

        // Setting uniform values to the currently bound program
        glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
        float color[] = { 1.f, 1.f, 1.f };
        glUniform3fv(color_uloc, 1, color);
        glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

        // Drawing!
        // glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, nullptr);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    }

    void incFrame() {
        int newIndex = index+1;
        if (newIndex >= totalSprites) newIndex = 0;
        index = newIndex;
    }

    void release() {
        glDeleteBuffers(1, &vertexDataBuffer);
        if( indexBuffers != nullptr ) {
            glDeleteBuffers(totalSprites, indexBuffers);
            delete[] indexBuffers;
        }
        glDeleteBuffers(1, &vao);
    }
};

#endif //VAPE_SPRITECOMPONENT_HPP
