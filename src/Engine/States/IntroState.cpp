//
// Created by Cody on 11/10/2019.
//

#include <sstream>
#include <Levels/Level1.hpp>
#include "IntroState.hpp"
#include "ControlsState.hpp"

void IntroState::init() {
    m_background_music_file.init(audio_path("intro.wav"));
    m_background_music = Load_Music(m_background_music_file);


    if (m_background_music == nullptr)
    {
        fprintf(stderr, "Failed to load sounds\n %s\n make sure the data directory is present",
                audio_path("intro.wav"));
        throw std::runtime_error("Failed to load sounds");
    }

    // Playing background music indefinitely
    Mix_PlayMusic(m_background_music, -1);

    // fprintf(stderr, "Loaded music\n");

    m_video.init(video_path("intro1.mp4"));
    m_skip.init();
}

void IntroState::terminate() {
    if (m_background_music != nullptr)
        Mix_FreeMusic(m_background_music);
    m_background_music_file.destroy();
    m_video.destroy();
    m_skip.destroy();
}

void IntroState::update(float ms) {
    m_video.update(ms);
    if (m_video.isOver())
        return changeState();
    m_skip.update(ms);
}

void IntroState::draw() {
    // Clearing error buffer
    gl_flush_errors();

    // Getting size of window
    int w, h;
    GLFWwindow* m_window = GameEngine::getInstance().getM_window();
    glfwGetFramebufferSize(m_window, &w, &h);

    // Updating window title with points
    std::stringstream title_ss;
    title_ss << "V.A.P.E";
    glfwSetWindowTitle(m_window, title_ss.str().c_str());

    // Clearing backbuffer
    glViewport(0, 0, w, h);
    glDepthRange(0.00001, 10);
    const float clear_color[3] = { 0.f, 0.f, 0.f };
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Fake projection matrix, scales with respect to window coordinates
    // PS: 1.f / w in [1][1] is correct.. do you know why ? (:
    float left = 0.f;// *-0.5;
    float top = 0.f;// (float)h * -0.5;
    float right = (float)w / GameEngine::getInstance().getM_screen_scale();// *0.5;
    float bottom = (float)h / GameEngine::getInstance().getM_screen_scale();// *0.5;

    float sx = 2.f / (right - left);
    float sy = 2.f / (top - bottom);
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    mat3 projection_2D{ { sx, 0.f, 0.f },{ 0.f, sy, 0.f },{ tx, ty, 1.f } };

    m_video.draw(projection_2D);
    m_skip.draw(projection_2D);

    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);
}

void IntroState::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
    if (action == GLFW_RELEASE && key == GLFW_KEY_ENTER)
    {
        return changeState();
    }
}

void IntroState::changeState() {
    GameEngine::getInstance().changeState(new ControlsState());
}