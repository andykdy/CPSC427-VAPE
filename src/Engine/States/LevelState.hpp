//! Game state representing Singleplayer gameplay
/*!
 * The SinglePlayerState represents the game state
 * for gameplay with one player
 *
 * @author: Cody Newman
 *
 * Created on: 9/17/2019
 */

#ifndef VAPE_LEVELSTATE_HPP
#define VAPE_LEVELSTATE_HPP

// internal
#include "common.hpp"
#include "Entities/Player.hpp"
#include "Entities/turtle.hpp"
#include "Entities/fish.hpp"
#include "Entities/Space.hpp"
#include "Entities/bullet.hpp"
#include "Entities/Dialogue.hpp"
#include "Entities/UI/Health.hpp"

// stlib
#include <vector>
#include <random>

#include <Engine/GameState.hpp>
#include <Entities/Vamp.hpp>
#include <Entities/Bosses/Boss1.hpp>
#include <Entities/UI/VampCharge.hpp>

class LevelState : public GameState {
public:
    //! Constructor, taking in gameplay options
    LevelState();

    //! Initializes the state
    void init() override;

    //! Cleans up the state
    void terminate() override;

    //! Updates the state of the level
    void update() override;

    //! Renders the state
    void draw() override;

    void on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) override;

    void on_mouse_move(GLFWwindow *window, double xpos, double ypos) override;

    void on_mouse_button(GLFWwindow *window, int button, int action, int mods) override;

private:
    void lose_health(int damage);
    void add_health(int heal);

    void add_vamp_charge();

    void reset(vec2 screen);

    // Generates a new turtle
    bool spawn_turtle();

    // Generates a new fish
    bool spawn_fish();

    // Tracks keys being pressed
    std::map<int, bool> keyMap;

    // Mouse position
    vec2 mouse_position;

    // Space effect
    Space m_space;

    // Number of fish eaten by the salmon, displayed in the window title
    unsigned int m_points;

    // Game entities
    Player* m_player;
    Boss1 m_boss;
    std::vector<Turtle*> *m_turtles;

    // UI
    Health* m_health;
    VampCharge* m_vamp_charge;
	Dialogue m_dialogue;

    // To remove
    std::vector<Fish> m_fish;

    float m_current_speed;
    float m_level_time;
    float m_next_turtle_spawn;
    float m_next_fish_spawn;

    bool m_boss_mode;

    // Vamp mode
    Vamp m_vamp;
    bool m_vamp_mode;
    float m_vamp_mode_timer;
    unsigned int m_vamp_mode_charge;



    Mix_Music* m_background_music;
    Mix_Music* m_boss_music;
    Mix_Music* m_victory_music;
    Mix_Chunk* m_player_dead_sound;
    Mix_Chunk* m_player_eat_sound;
    Mix_Chunk* m_player_explosion;
    Mix_Chunk* m_player_charged;

    // C++ rng
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist; // default 0..1
};


#endif //VAPE_LEVELSTATE_HPP
