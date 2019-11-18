//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_INTROSTATE_HPP
#define VAPE_INTROSTATE_HPP


#include <Engine/GameState.hpp>
#include <Entities/Intro.hpp>
#include <Entities/UI/EnterSkip.hpp>

class IntroState : public GameState{
public:
    void init() override;

    void terminate() override;

    void update(float ms) override;

    void draw() override;

    void on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) override;

    void on_mouse_move(GLFWwindow *window, double xpos, double ypos) override {};

    void on_mouse_button(GLFWwindow *window, int button, int action, int mods) override {};

private:
    Video m_video;
    EnterSkip m_skip;
    // Intro* m_intro;
    Mix_Music* m_background_music;
};


#endif //VAPE_INTROSTATE_HPP
