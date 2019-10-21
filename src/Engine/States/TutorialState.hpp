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
#include "Entities/turtle.hpp"
#include "Entities/Space.hpp"
#include "Entities/bullet.hpp"
#include "Entities/UI/Health.hpp"
#include "Entities/Dialogue.hpp"

// stlib
#include <vector>
#include <random>

#include <Engine/GameState.hpp>
#include <Entities/Vamp.hpp>

enum Component;

class TutorialState : public GameState {
	static Texture dlg_texture;
public:
	//! Constructor, taking in gameplay options
	TutorialState();

	//! Initializes the state
	void init(GameEngine *game) override;

	//! Cleans up the state
	void terminate() override;

	//! Updates the state of the level
	void update(GameEngine *game) override;

	//! Renders the state
	void draw(GameEngine *game) override;

	void on_key(GameEngine *game, GLFWwindow *wwindow, int key, int i, int action, int mod) override;

	void on_mouse_move(GameEngine *game, GLFWwindow *window, double xpos, double ypos) override;

	void on_mouse_button(GameEngine *game, GLFWwindow *window, int button, int action, int mods) override;

private:
	void lose_health(int damage);
	void add_health(int heal);

	void reset(vec2 screen);

	// Generates a new turtle
	bool spawn_turtle();

	// Tracks keys being pressed
	std::map<int, bool> keyMap;

	// Mouse position
	vec2 mouse_position;

	// Space effect
	Space m_space;

	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int m_points;

	// Game entities
	Player m_player;
	Health m_health;
	std::vector<Turtle> m_turtles;
	Dialogue m_dialogue;

	float m_current_speed;
	float m_level_start;
	float m_next_turtle_spawn;
	float m_next_fish_spawn;

	bool m_spawn_enemies;
	bool m_boss_mode;

	Component m_current_cmp;
	bool m_mvmt_checklist[4];
	float m_vamp_quota;


	// Vamp mode
	Vamp m_vamp;
	bool m_vamp_mode;
	float m_vamp_mode_timer;
	unsigned int m_vamp_mode_charge;

	Mix_Music* m_background_music;
	Mix_Chunk* m_player_dead_sound;
	Mix_Chunk* m_player_eat_sound;
	Mix_Chunk* m_player_explosion;

	// C++ rng
	std::default_random_engine m_rng;
	std::uniform_real_distribution<float> m_dist; // default 0..1
};


#endif //VAPE_TUTORIAL_HPP
