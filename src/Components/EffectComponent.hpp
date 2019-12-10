//
// Created by Cody on 10/16/2019.
//

#include <common.hpp>
#include <Engine/ECS/ECS.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <physfs.hpp>

#ifndef VAPE_EFFECTCOMPONENT_HPP
#define VAPE_EFFECTCOMPONENT_HPP

namespace
{
    bool gl_compile_shader(GLuint shader)
    {
        glCompileShader(shader);
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLint log_len;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
            std::vector<char> log(log_len);
            glGetShaderInfoLog(shader, log_len, &log_len, log.data());
            glDeleteShader(shader);

            fprintf(stderr, "GLSL: %s", log.data());
            return false;
        }

        return true;
    }
}

class EffectComponent : public ECS::Component {
private:

public:
    GLuint vertex;
    GLuint fragment;
    GLuint program;

    void release()
    {
        if (program != 0) {
            glDeleteProgram(program);
            program = 0;
        }
        if (vertex != 0) {
            glDeleteShader(vertex);
            vertex = 0;
        }
        if (fragment != 0) {
            glDeleteShader(fragment);
            fragment = 0;
        }
    }

    bool load_from_file(const char* vs_path, const char* fs_path)
    {
        gl_flush_errors();

        // Opening files
        auto* vs_is = new PhysFS::ifstream(vs_path);
        auto* fs_is = new PhysFS::ifstream(fs_path);

        if (!vs_is->good() || !fs_is->good())
        {
            fprintf(stderr, "Failed to load shader files %s, %s", vs_path, fs_path);
            return false;
        }

        // Reading sources
        std::stringstream vs_ss, fs_ss;
        vs_ss << vs_is->rdbuf();
        fs_ss << fs_is->rdbuf();
        delete vs_is;
        delete fs_is;
        std::string vs_str = vs_ss.str();
        std::string fs_str = fs_ss.str();
        const char* vs_src = vs_str.c_str();
        const char* fs_src = fs_str.c_str();
        GLsizei vs_len = (GLsizei)vs_str.size();
        GLsizei fs_len = (GLsizei)fs_str.size();

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vs_src, &vs_len);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fs_src, &fs_len);

        // Compiling
        // Shaders already delete if compilation fails
        if (!gl_compile_shader(vertex))
            return false;

        if (!gl_compile_shader(fragment))
        {
            glDeleteShader(vertex);
            return false;
        }

        // Linking
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        {
            GLint is_linked = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
            if (is_linked == GL_FALSE)
            {
                GLint log_len;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
                std::vector<char> log(log_len);
                glGetProgramInfoLog(program, log_len, &log_len, log.data());

                release();
                fprintf(stderr, "Link error: %s", log.data());
                return false;
            }
        }

        if (gl_has_errors())
        {
            release();
            fprintf(stderr, "OpenGL errors occured while compiling Effect");
            return false;
        }

        if (vertex != 0) {
            glDeleteShader(vertex);
            vertex = 0;
        }
        if (fragment != 0) {
            glDeleteShader(fragment);
            fragment = 0;
        }

        return true;
    }
};


#endif //VAPE_EFFECTCOMPONENT_HPP