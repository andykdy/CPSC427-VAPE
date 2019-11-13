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
#include "Entities/Enemies/turtle.hpp"
//#include "Entities/Prototype.hpp"
#include "Entities/fish.hpp"
#include "Entities/Space.hpp"
#include "Entities/Projectiles and Damaging/bullet.hpp"
#include "Entities/UI/Dialogue.hpp"
#include "Entities/UI/PlayerHealth/Health.hpp"
#include "Entities/Explosion.hpp"
#include "Levels/Level.hpp"

// stlib
#include <vector>
#include <random>

#include <Engine/GameState.hpp>
#include <Entities/Vamp.hpp>
#include <Entities/Bosses/Boss1.hpp>
#include <Entities/UI/Vamp/VampCharge.hpp>
#include <Entities/UI/UIPanelBackground.hpp>
#include <Entities/UI/UIPanel.hpp>
#include <Entities/Pickups/Pickup.hpp>

class Pickup;

class LevelState : public GameState {
    friend class Pickup;
public:
    //! Constructor, taking in gameplay options
    explicit LevelState(Levels::Level level, unsigned int points);

    //! Initializes the state
    void init() override;

    //! Cleans up the state
    void terminate() override;

    //! Updates the state of the level
    void update(float ms) override;

    //! Renders the state
    void draw() override;

    void on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) override;

    void on_mouse_move(GLFWwindow *window, double xpos, double ypos) override;

    void on_mouse_button(GLFWwindow *window, int button, int action, int mods) override;

private:
    Levels::Level m_level;

    void lose_health(int damage);
    void add_health(int heal);

    void add_vamp_charge();

    void reset();

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
    Boss* m_boss;
    std::vector<Pickup*> m_pickups; // TODO Maybe should be in a Pickup System eventuallyy
    std::vector<Enemy*> *m_turtles;

    // UI
    UIPanelBackground* m_uiPanelBackground;
    UIPanel* m_uiPanel;
    Health* m_health;
    VampCharge* m_vamp_charge;
	Dialogue m_dialogue;

    float m_level_time;
    float m_vamp_cooldown;

    bool m_boss_pre;
    bool m_boss_mode;

    // Vamp mode
    Vamp m_vamp;
    bool m_vamp_mode;
    float m_vamp_mode_timer;
    unsigned int m_vamp_mode_charge;
    Explosion m_explosion;



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
