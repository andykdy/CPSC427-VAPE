//
// Created by Cody on 9/2/2019.
//

#include <cmath>
#include <algorithm>
#include "Vamp.hpp"

bool Vamp::init(vec2 position, float rotation) {

    m_vertices.clear();
    m_indices.clear();

    GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * M_PI;

    // Reading vertices and colors
    // size_t num_vertices;
    for (size_t i = 0; i < 359; ++i)
    {

        Vertex vertex;
        vertex.position = {static_cast<float>(position.x + (radius * cos(  i * twicePi / 360))),
                           static_cast<float>(position.y + (radius * sin(  i * twicePi / 360))),
                           -0.05f };
        vertex.color = { (float)255 / 255, (float)0 / 255, (float)0 / 255 };
        m_vertices.push_back(vertex);

    }

    // Reading associated indices
    size_t num_indices;

    // uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };

    for (size_t i = 0; i < 361; ++i)
    {
        m_indices.push_back((uint16_t) 0 );
        m_indices.push_back((uint16_t) i + 1 );
        m_indices.push_back((uint16_t) i + 2 );
    }

//    fscanf(mesh_file, "%zu\n", &num_indices);
//    for (size_t i = 0; i < num_indices; ++i)
//    {
//        int idx[3];
//        fscanf(mesh_file, "%d %d %d\n", idx, idx + 1, idx + 2);
//        m_indices.push_back((uint16_t)idx[0]);
//        m_indices.push_back((uint16_t)idx[1]);
//        m_indices.push_back((uint16_t)idx[2]);
//    }

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("salmon.vs.glsl"), shader_path("salmon.fs.glsl")))
        return false;

    // Setting initial values
    motion.position = { position.x, position.y };
    motion.radians = 0.f;
    motion.speed = 200.f;

    physics.scale = { -35.f, 35.f };

    return true;





/*    // ---------------------------------------------------------------


//    int i;
//    GLfloat triangleAmount = 360.f;
//
//    GLfloat radius = 0.8f; //radius
//    GLfloat twicePi = 2.0f * M_PI;
//
//    // from VAMP
//    TexturedVertex vertices[360];
//
//    //glVertex2f(x, y); // center of circle
//
//
//    for(i = 0; i <= triangleAmount; i++) {
//        vertices[i].position = {static_cast<float>(position.x + (radius * cos(  i * twicePi / triangleAmount))),
//                                static_cast<float>(position.y + (radius * sin(  i * twicePi / triangleAmount))),
//                                -0.05f };
//    }
//
//    // ---------------------------------------------------------------
//
//
//    // The position corresponds to the center of the texture
//    float wr = vamp_texture.width * 0.5f;
//    float hr = vamp_texture.height * 0.5f;

//    vertices[0].position = { -wr, +hr, -0.05f };
//    vertices[0].texcoord = { 0.f, 1.f };
//    vertices[1].position = { +wr, +hr, -0.05f };
//    vertices[1].texcoord = { 1.f, 1.f };
//    vertices[2].position = { +wr, -hr, -0.05f };
//    vertices[2].texcoord = { 1.f, 0.f };
//    vertices[3].position = { -wr, -hr, -0.05f };
//    vertices[3].texcoord = { 0.f, 0.f };

    // counterclockwise as it's the default opengl front winding direction
    uint16_t indices[] = { 0, 360, 120, 120, 360, 240 };
    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("vamp.vs.glsl"), shader_path("vamp.fs.glsl")))
        return false;


    m_scale.x = 1.3f;
    m_scale.y = 1.3f;

    m_rotation = rotation;

    m_position.x = position.x;
    m_position.y = position.y;

    return true;*/
}

void Vamp::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Vamp::update(float ms, vec2 player_position) {
    m_position.x = player_position.x;
    m_position.y = player_position.y;
}

void Vamp::draw(const mat3 &projection) {
    transform.begin();

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // SALMON TRANSFORMATION CODE HERE

    // see Transformations and Rendering in the specification pdf
    // the following functions are available:
    transform.translate(motion.position);
    // rotate()
    transform.scale(physics.scale);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // REMOVE THE FOLLOWING LINES BEFORE ADDING ANY TRANSFORMATION CODE
    // transform.translate({ 100.0f, 100.0f });
    // transform.scale(physics.scale);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    transform.end();

    // Setting shaders
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Getting uniform locations
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");
    GLint light_up_uloc = glGetUniformLocation(effect.program, "light_up");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_color_loc = glGetAttribLocation(effect.program, "in_color");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_color_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);

    // !!! Salmon Color
    float color[] = { 1.f, 0.f, 0.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // HERE TO SET THE CORRECTLY LIGHT UP THE SALMON IF HE HAS EATEN RECENTLY
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    int light_up = 0;
    glUniform1iv(light_up_uloc, 1, &light_up);

    // Get number of infices from buffer,
    // we know our vbo contains both colour and position information, so...
    GLint size = 0;
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    GLsizei num_indices = size / sizeof(uint16_t);

    // Drawing!
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);



   /* // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform.begin();
    transform.translate(m_position);
    transform.rotate(m_rotation);
    transform.scale(m_scale);
    transform.end();

    // Setting shaders
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
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
    glBindTexture(GL_TEXTURE_2D, vamp_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
    float color[] = { 0.f, 0.f, 1.f, 0.2f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);*/
}

vec2 Vamp::get_position()const
{
    return m_position;
}

bool Vamp::collides_with(const Turtle &turtle) {
//    float dx = m_position.x - turtle.get_position().x;
//    float dy = m_position.y - turtle.get_position().y;
//    float d_sq = dx * dx + dy * dy;
//    float other_r = std::max(turtle.get_bounding_box().x, turtle.get_bounding_box().y);
//    float my_r = std::max(vamp_texture.width * m_scale.x,  vamp_texture.height * m_scale.y);
//    float r = std::max(other_r, my_r);
//    r *= 0.6f;
//    if (d_sq < r * r)
//        return true;
//    return false;
    return false;
}

vec2 Vamp::get_bounding_box()const
{
    // return { std::fabs(m_scale.x) * vamp_texture.width, std::fabs(m_scale.y) * vamp_texture.height };
    return {};
}
