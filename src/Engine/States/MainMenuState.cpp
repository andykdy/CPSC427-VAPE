//
// Created by Cody on 9/17/2019.
//

#include <sstream>
#include <Levels/Levels.hpp>
#include "MainMenuState.hpp"
#include "LevelState.hpp"
#include "TutorialState.hpp"
#include "IntroState.hpp"


void MainMenuState::init() {
    m_background_music = Mix_LoadMUS(audio_path("mainmenu.wav"));

    // Playing background music indefinitely
    Mix_PlayMusic(m_background_music, -1);

    menu.init();


    // TODO setup buttons
}

void MainMenuState::terminate() {
    if (m_background_music != nullptr)
        Mix_FreeMusic(m_background_music);

    for (auto& button : m_buttons)
        button->destroy();
    m_buttons.clear();
    
    menu.destroy();
}

void MainMenuState::update(float ms) {
    for (auto& button : m_buttons) {
        button->update(ms, mouse_position);
    }
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

    for (auto& button : m_buttons) {
        button->draw(projection_2D);
    }
    // TODO draw cursor beside targetted button? or have the button highlighted?

    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);

}

void MainMenuState::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
    if (action == GLFW_RELEASE && key == GLFW_KEY_UP) {
        --m_button_cursor;
        if (m_button_cursor < 0)
            m_button_cursor = 0;
    }
    if (action == GLFW_RELEASE && key == GLFW_KEY_DOWN) {
        ++m_button_cursor;
        if (m_button_cursor > m_buttons.size()-1)
            m_button_cursor = m_buttons.size()-1;
    }
    if (action == GLFW_RELEASE && key == GLFW_KEY_ENTER) {
        if (m_buttons.size() > 0 && m_button_cursor > 0 && m_button_cursor < m_buttons.size()-1) {
            m_buttons[m_button_cursor]->doAction();
        }
    }



    if (action == GLFW_RELEASE && key == GLFW_KEY_1)
    {
        GameEngine::getInstance().changeState(new LevelState(Levels::level1, 0));
    }
    if (action == GLFW_RELEASE && key == GLFW_KEY_2)
    {
        GameEngine::getInstance().changeState(new LevelState(Levels::level2, 0));
    }
}

void MainMenuState::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
	mouse_position.x = xpos;
	mouse_position.y = ypos;
}

void MainMenuState::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_RELEASE) {
        for (auto& button : m_buttons) {
            if (button->isClicked(mouse_position)) {
                button->doAction();
                break;
            }
        }
    }


    // TODO remove
	if (mouse_position.x >= 200 && mouse_position.x <= 600 && mouse_position.y >= 600 && mouse_position.y <= 700) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			GameEngine::getInstance().changeState(new IntroState());
		}
	}
	if (mouse_position.x >= 280 && mouse_position.x <= 520 && mouse_position.y >= 740 && mouse_position.y <= 830) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			GameEngine::getInstance().quit();
		}
	}
}
