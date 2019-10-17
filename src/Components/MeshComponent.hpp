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
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &vao);
    }
};

#endif //VAPE_MESHCOMPONENT_HPP
