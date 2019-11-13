//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_MESHCOMPONENT_HPP
#define VAPE_MESHCOMPONENT_HPP

#include <Engine/ECS/ECS.hpp>

class MeshComponent : public ECS::Component {
public:
    GLuint vao;
    GLuint vbo;
    GLuint ibo;

    void release() {
        if (vbo != 0){
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }

        if( ibo != 0 ) {
            glDeleteBuffers(1, &ibo);
            ibo = 0;
        }

        if (vao != 0){
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }
    }
};

#endif //VAPE_MESHCOMPONENT_HPP
