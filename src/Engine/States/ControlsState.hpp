//
// Created by Cody on 11/19/2019.
//

#ifndef VAPE_CONTROLSSTATE_HPP
#define VAPE_CONTROLSSTATE_HPP

#include <Engine/GameState.hpp>
#include <Entities/UI/PauseMenu/ControlDiagram.hpp>
#include <Levels/Level1.hpp>
#include <sstream>
#include <Entities/UI/EnterContinue.hpp>
#include "LevelState.hpp"

// Just displays the controls before starting the game
class ControlsState : public GameState{
public:
    void init() override {
        int w, h;
        glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
        vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

        m_controls = &GameEngine::getInstance().getEntityManager()->addEntity<ControlDiagram>();
        m_controls->init({screen.x/2, screen.y*0.5f}, {0.7f,0.7f}, 0);

        m_continue.init();
    }

    void terminate() override {
        m_controls->destroy();
        m_continue.destroy();
    }

    void update(float ms) override {

    }

    void draw() override {
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

        m_controls->draw(projection_2D);
        m_continue.draw(projection_2D);

        //////////////////
        // Presenting
        glfwSwapBuffers(m_window);
    }

    void on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) override {
        if (action == GLFW_RELEASE && key == GLFW_KEY_ENTER)
        {
            GameEngine::getInstance().changeState(new LevelState(Levels::level1, {INIT_LIVES,0,0}));
        }
    };

    void on_mouse_move(GLFWwindow *window, double xpos, double ypos) override {};

    void on_mouse_button(GLFWwindow *window, int button, int action, int mods) override {};

private:
    ControlDiagram* m_controls;
    EnterContinue m_continue;
};

#endif //VAPE_CONTROLSSTATE_HPP
