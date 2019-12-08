//! Game state representing the tutorial leve
/*!
 * Tutorial level that precedes the main game
 *
 * @author: Andy Kim
 *
 * Created on: 10/20/2019
 */

#ifndef VAPE_TUTORIAL_HPP
#define VAPE_TUTORIAL_HPP

  // internal
#include "common.hpp"
#include "Entities/Player.hpp"
#include "Entities/Enemies/turtle.hpp"
#include "Entities/Space.hpp"
#include "Entities/Projectiles and Damaging/bullet.hpp"
#include "Entities/UI/PlayerHealth/Health.hpp"
#include "Entities/UI/Dialogue/Dialogue.hpp"
#include "Entities/UI/Dialogue/Continue.hpp"

#include <Entities/UI/ScoreText.hpp>
#include <Entities/UI/PlayerScore/Score.hpp>
#include <Entities/UI/PlayerScore/ScoreBackground.hpp>
#include <Entities/UI/Lives/LivesBackground.hpp>
#include <Entities/UI/Lives/Lives.hpp>
#include <Entities/UI/Weapon/WeaponUI.hpp>

// stlib
#include <vector>
#include <random>

#include <Engine/GameState.hpp>
#include <Entities/Vamp.hpp>
#include <Entities/UI/Vamp/VampCharge.hpp>
#include <Entities/Effects/Explosion.hpp>
#include <Entities/UI/UIPanel/UIPanelBackground.hpp>
#include <Entities/UI/UIPanel/UIPanel.hpp>
#include <Entities/UI/PauseMenu/PauseMenu.hpp>
#include <Entities/Effects/VampParticleEmitter.hpp>
#include <Utils/PhysFSHelpers.hpp>

enum Component {
	initial,
	movement,
	shooting,
	vamp_1,
	vamp_2,
	clear
};

class TutorialState : public GameState {
public:
	//! Constructor, taking in gameplay options
	TutorialState();

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
	PauseMenu* m_pause;
	void add_vamp_charge();

	void lose_health(int damage);
	void add_health(int heal);
    void spawn_score_text(int pts, vec2 position);

	void reset();

	// Generates a new turtle
	bool spawn_turtle();

	// Tracks keys being pressed
	std::map<int, bool> keyMap;

	// Mouse position
	vec2 mouse_position;

	// Space effect
	Space m_space;

    unsigned int m_lives;
	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int m_points;

	// Game entities
	Player* m_player;
	Health* m_health;
	VampCharge* m_vamp_charge;
	std::vector<Turtle*> m_turtles;
	Dialogue m_dialogue;
	Continue m_continue_UI;
    UIPanelBackground* m_uiPanelBackground;
    UIPanel* m_uiPanel ;
    Score* m_score_ui;
    ScoreBackground* m_score_background;
    Lives* m_lives_ui;
    LivesBackground* m_lives_background;
    WeaponUI* m_weapon_ui;

    // Text rendering
    Font m_font_ranger;
    Font m_font_condensed;
    // Font m_font_scoring;
    std::vector<Text> m_text;
    std::vector<ScoreText> m_score_text;

    bool m_debug_mode;
    bool m_player_invincibility;

	float m_next_turtle_spawn;
	float m_next_fish_spawn;

	Component m_current_cmp;
	bool m_mvmt_checklist[4];
	float m_vamp_quota;


	// Vamp mode
	Vamp m_vamp;
    bool m_vamp_mode;
    float m_vamp_mode_timer;
    float m_vamp_cooldown;
    unsigned int m_vamp_mode_charge;
    int m_numVampParticles;

    // Effects
    VampParticleEmitter m_vamp_particle_emitter;
    Explosion m_explosion;

	RWFile m_background_music_file;
	Mix_Music* m_background_music;
	RWFile m_player_dead_sound_file;
	Mix_Chunk* m_player_dead_sound;
	RWFile m_player_eat_sound_file;
	Mix_Chunk* m_player_eat_sound;
	RWFile m_player_explosion_file;
	Mix_Chunk* m_player_explosion;
	RWFile m_player_charged_file;
	Mix_Chunk* m_player_charged;

	// C++ rng
	std::default_random_engine m_rng;
	std::uniform_real_distribution<float> m_dist; // default 0..1
};


#endif //VAPE_TUTORIAL_HPP
