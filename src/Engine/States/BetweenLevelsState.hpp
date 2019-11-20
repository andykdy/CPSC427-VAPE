//
// Created by Cody on 11/20/2019.
//

#ifndef VAPE_BETWEENLEVELSSTATE_HPP
#define VAPE_BETWEENLEVELSSTATE_HPP


#include <Engine/GameState.hpp>
#include <Levels/Levels.hpp>
#include <Utils/SaveData.hpp>
#include <Entities/UI/EnterContinue.hpp>

class BetweenLevelsState : public GameState {
private:
    const Levels::Level* m_next_level;
    unsigned int m_prev_start_points;
    PlayerData m_player_data;
    EnterContinue m_continue;
public:
    BetweenLevelsState(const Levels::Level *m_next_level, unsigned int m_prev_start_points, const PlayerData &m_player_data);

    void init() override;

    void terminate() override;

    void update(float ms) override;

    void draw() override;

    void on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) override;

    void on_mouse_move(GLFWwindow *window, double xpos, double ypos) override;

    void on_mouse_button(GLFWwindow *window, int button, int action, int mods) override;
};


#endif //VAPE_BETWEENLEVELSSTATE_HPP
