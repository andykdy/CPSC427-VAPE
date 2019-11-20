//
// Created by Cody on 9/17/2019.
//

#include <sstream>
#include <Levels/Levels.hpp>
#include <Entities/UI/MainMenu/ContinueButton.hpp>
#include <Entities/UI/MainMenu/StartButton.hpp>
#include <Entities/UI/MainMenu/TutorialButton.hpp>
#include <Entities/UI/MainMenu/ExitButton.hpp>
#include "MainMenuState.hpp"
#include "LevelState.hpp"
#include "TutorialState.hpp"
#include "IntroState.hpp"

void MainMenuState::init() {
    m_background_music = Mix_LoadMUS(audio_path("mainmenu.wav"));

    // Playing background music indefinitely
    Mix_PlayMusic(m_background_music, -1);

    menu.init();

    int w, h;
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

    vec2 buttonpos = {screen.x *0.75f, screen.y *0.3f};
    vec2 buttonscale = {0.75f, 0.75f};
    float buttonHeight = 150.f; // TODO
    float offset = 20.f;


    m_cursor = &GameEngine::getInstance().getEntityManager()->addEntity<Cursor>();
    m_cursor->init({buttonpos.x- 210, buttonpos.y}, buttonscale, 0);


    auto* continue_button = &GameEngine::getInstance().getEntityManager()->addEntity<ContinueButton>();
    continue_button->init(buttonpos, buttonscale, 0);
    continue_button->select();
    m_buttons.push_back(continue_button);

    buttonpos.y += buttonHeight + offset;

    auto* start_button = &GameEngine::getInstance().getEntityManager()->addEntity<StartButton>();
    start_button->init(buttonpos, buttonscale, 0);
    m_buttons.push_back(start_button);

    buttonpos.y += buttonHeight + offset;

    auto* tutorial_button = &GameEngine::getInstance().getEntityManager()->addEntity<TutorialButton>();
    tutorial_button->init(buttonpos, buttonscale, 0);
    m_buttons.push_back(tutorial_button);

    buttonpos.y += buttonHeight + offset;

    auto* exit_button = &GameEngine::getInstance().getEntityManager()->addEntity<ExitButton>();
    exit_button->init(buttonpos, buttonscale, 0);
    m_buttons.push_back(exit_button);

    buttonpos.y += buttonHeight + offset;

    m_button_cursor = 0;

}

void MainMenuState::terminate() {
    if (m_background_music != nullptr)
        Mix_FreeMusic(m_background_music);

    for (auto& button : m_buttons)
        button->destroy();
    m_buttons.clear();

    m_cursor->destroy();

    menu.destroy();
}

void MainMenuState::update(float ms) {
    for (auto& button : m_buttons) {
        button->update(ms, mouse_position);
    }
}

void MainMenuState::draw() {
    if (!m_buttons.empty() && m_button_cursor >= 0 && m_button_cursor < m_buttons.size()) {
        vec2 pos = m_buttons[m_button_cursor]->getPosition();
        m_cursor->setPosition({pos.x-210, pos.y}); // TODO
    } else {
        m_cursor->setPosition({-200, -200});
    }



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
    m_cursor->draw(projection_2D);

    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);

}

void MainMenuState::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
    if (!m_buttons.empty()) {
        if (m_button_cursor >= 0 && m_button_cursor < m_buttons.size()) {
            m_buttons[m_button_cursor]->delesect();
        }

        if (action == GLFW_RELEASE && (key == GLFW_KEY_UP || key == GLFW_KEY_W)) {
            if (m_button_cursor == 0) {
                m_button_cursor = m_buttons.size() - 1;
            } else
                --m_button_cursor;
        }
        if (action == GLFW_RELEASE && (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)) {
            ++m_button_cursor;
            if (m_button_cursor > m_buttons.size() - 1)
                m_button_cursor = 0;
        }

        if (m_button_cursor >= 0 && m_button_cursor < m_buttons.size() ) {
            m_buttons[m_button_cursor]->select();
        }

        if (action == GLFW_RELEASE && key == GLFW_KEY_ENTER) {
            if (m_button_cursor >= 0 && m_button_cursor < m_buttons.size()) {
                m_buttons[m_button_cursor]->doAction();
            }
        }
    }



    if (action == GLFW_RELEASE && key == GLFW_KEY_1)
    {
        GameEngine::getInstance().changeState(new LevelState(Levels::level1, {INIT_LIVES,0,0}));
    }
    if (action == GLFW_RELEASE && key == GLFW_KEY_2)
    {
        GameEngine::getInstance().changeState(new LevelState(Levels::level2, {INIT_LIVES,0,0}));
    }
}

void MainMenuState::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
	mouse_position.x = xpos;
	mouse_position.y = ypos;
}

void MainMenuState::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_RELEASE) {
        for (auto& button : m_buttons) {
            if (button->isWithin(mouse_position)) {
                button->doAction();
                break;
            }
        }
    }
}
