/*!
 * @author: Cody Newman
 *
 * Created on: 9/16/2019
 */

#include "GameEngine.hpp"
#include "GameState.hpp"

// internal
#include "common.hpp"

#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>
#include <iostream>

// Same as static in c, local to compilation unit
namespace
{
    const size_t SCREEN_WIDTH = 800;
    const size_t SCREEN_HEIGHT = 1000;

    namespace
    {
        void glfw_err_cb(int error, const char* desc)
        {
            fprintf(stderr, "%d: %s", error, desc);
        }
    }
}

/*!
 * Initialize the game engine, setting up GLFW
 */
void GameEngine::init() {
    //-------------------------------------------------------------------------
    // GLFW / OGL Initialization
    // Core Opengl 3.
    glfwSetErrorCallback(glfw_err_cb);
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    #if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, 0);
    m_window = glfwCreateWindow((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, "V.A.P.E", nullptr, nullptr);
    if (m_window == nullptr)
        throw std::runtime_error("No window?");

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // vsync

    // Load OpenGL function pointers
    gl3w_init();

    // Setting callbacks to member functions (that's why the redirect is needed)
    // Input is handled using GLFW, for more info see
    // http://www.glfw.org/docs/latest/input_guide.html
    glfwSetWindowUserPointer(m_window, this);
    auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((GameEngine*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
    auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((GameEngine*)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
    auto mouse_button_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2) { ((GameEngine*)glfwGetWindowUserPointer(wnd))->on_mouse_button(wnd, _0, _1, _2); };
    glfwSetKeyCallback(m_window, key_redirect);
    glfwSetCursorPosCallback(m_window, cursor_pos_redirect);
    glfwSetMouseButtonCallback(m_window, mouse_button_redirect);

    // Create a frame buffer
    m_frame_buffer = 0;
    glGenFramebuffers(1, &m_frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

    // For some high DPI displays (ex. Retina Display on Macbooks)
    // https://stackoverflow.com/questions/36672935/why-retina-screen-coordinate-value-is-twice-the-value-of-pixel-value
    int fb_width, fb_height;
    glfwGetFramebufferSize(m_window, &fb_width, &fb_height);
    m_screen_scale = static_cast<float>(fb_width) / SCREEN_WIDTH;

    // Initialize the screen texture
    m_screen_tex.create_from_screen(m_window);

    //-------------------------------------------------------------------------
    // Loading music and sounds
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        throw std::runtime_error("Failed to initialize SDL Audio");
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        throw std::runtime_error("Failed to open audio device");
    }

    m_current_speed = 1.f;

    running = true;
}

/*!
 * Cleans up the engine, terminating the running state, deleting any allocated objects
 */
void GameEngine::terminate() {
    glDeleteFramebuffers(1, &m_frame_buffer);

    // Clean up the current state
    if (state != nullptr){
        state->terminate();
    }
    delete(state);
    state = nullptr;

    //Free the music
    Mix_FreeMusic(music);
    music = nullptr;
    Mix_CloseAudio();

    glfwDestroyWindow(m_window);
    m_window = nullptr;

    // Quit SDL
    Mix_Quit();
}

/*!
 * Terminates the current state and replaces it with the
 * provided GameState
 * @param state the new GameState to be swapped in
 */
void GameEngine::changeState(GameState *state) {
    if (this->state != nullptr) {
        this->state->terminate();
        delete (this->state);
    }
    this->state = state;
    state->init();
}

/*!
 * Runs the state's update function
 */
void GameEngine::update(float ms) {
    //entityManager.update(ms);
    systemManager.update(ms * m_current_speed);
    this->elapsed_ms = ms;
    state->update(ms * m_current_speed);

    if (glfwWindowShouldClose(m_window)) {
        this->running = false;
    }
}

/*!
 * Runs the state's render function
 */
void GameEngine::draw() {
    //entityManager.draw(...);
    //systemManager.draw(...);
    state->draw();
}

/*!
 * Runs the state's key handler
 */
void GameEngine::on_key(GLFWwindow *window, int key, int i, int action, int mod) {
    state->on_key(window, key, i, action, mod);
}

/*!
 * Runs the state's mouse movement handler
 */
void GameEngine::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
    state->on_mouse_move(window, xpos, ypos);
}

/*!
 * Runs the state's mouse button handler
 */
void GameEngine::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
    state->on_mouse_button(window, button, action, mods);
}



float GameEngine::getElapsed_ms() const {
    return elapsed_ms;
}

GLFWwindow *GameEngine::getM_window() const {
    return m_window;
}

float GameEngine::getM_screen_scale() const {
    return m_screen_scale;
}

GLuint GameEngine::getM_frame_buffer() const {
    return m_frame_buffer;
}

const Texture &GameEngine::getM_screen_tex() const {
    return m_screen_tex;
}

float GameEngine::getM_current_speed() const {
    return m_current_speed;
}

void GameEngine::setM_current_speed(float m_current_speed) {
    GameEngine::m_current_speed = m_current_speed;
}

ECS::EntityManager *GameEngine::getEntityManager() {
    return &entityManager;
}
ECS::SystemManager *GameEngine::getSystemManager() {
    return &systemManager;
}
