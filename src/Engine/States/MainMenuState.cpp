//
// Created by Cody on 9/17/2019.
//

#include <sstream>
#include "MainMenuState.hpp"
#include "LevelState.hpp"
#include "TutorialState.hpp"

Texture MainMenuState::bg_texture;

void MainMenuState::init() {
    if (!bg_texture.is_valid())
    {
        if (!bg_texture.load_from_file(textures_path("mainmenu_bg.png")))
        {
            throw std::runtime_error("Failed to load background texture!");
        }
    }

    m_background_music = Mix_LoadMUS(audio_path("mainmenu.wav"));

    // Playing background music indefinitely
    Mix_PlayMusic(m_background_music, -1);

    menu.init();
}

void MainMenuState::terminate() {
    if (m_background_music != nullptr)
        Mix_FreeMusic(m_background_music);
    menu.destroy();
}

void MainMenuState::update(float ms) {

}

void MainMenuState::draw() {
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

    menu.draw(projection_2D);

    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);

}

void MainMenuState::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
}

void MainMenuState::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
	mouse_position.x = xpos;
	mouse_position.y = ypos;
}

void MainMenuState::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
	if (mouse_position.x >= 200 && mouse_position.x <= 600 && mouse_position.y >= 600 && mouse_position.y <= 700) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		    // GameEngine::getInstance().getEntityManager()->reset();
			GameEngine::getInstance().changeState(new TutorialState());
		}
	}
}
