/*!
 * @author: Andy Kim
 *
 * Created on: 10/20/2019
 */

 // stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <algorithm>

#include "TutorialState.hpp"
#include "MainMenuState.hpp"
#include "LevelState.hpp"

// Same as static in c, local to compilation unit
namespace
{
	const size_t VAMP_MODE_DURATION = 1500;
	const size_t MAX_HEALTH = 75;
	const size_t INIT_HEALTH = 50;
	const size_t DAMAGE_ENEMY = 5;
	const size_t DAMAGE_BOSS = 5;
	const size_t BOSS_TIME = 30;
	const size_t VAMP_HEAL = 2;
	const size_t VAMP_KILLS_NEEDED = 3;
}


TutorialState::TutorialState() :
	m_points(0),
	m_next_turtle_spawn(0.f),
	m_next_fish_spawn(0.f)
{
	// Seeding rng with random device
	m_rng = std::default_random_engine(std::random_device()());
}

void TutorialState::init(GameEngine *game) {
	m_background_music = Mix_LoadMUS(audio_path("music.wav"));
	m_player_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav"));
	m_player_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav"));
	m_player_explosion = Mix_LoadWAV(audio_path("explosion.wav"));

	if (m_background_music == nullptr || m_player_dead_sound == nullptr || m_player_eat_sound == nullptr)
	{
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav"),
			audio_path("salmon_dead.wav"),
			audio_path("salmon_eat.wav"));
		throw std::runtime_error("Failed to load sounds");
	}

	// Playing background music indefinitely
	Mix_PlayMusic(m_background_music, -1);

	fprintf(stderr, "Loaded music\n");

	// Get screen size
	int w, h;
	glfwGetFramebufferSize(game->getM_window(), &w, &h);
	vec2 screen = { (float)w / game->getM_screen_scale(), (float)h / game->getM_screen_scale() };

	m_current_speed = 1.f;
	m_level_start = glfwGetTime();

	m_player.init(screen, INIT_HEALTH);
	m_health.init({ 45, 60 });
	m_space.init();
	m_dialogue.init();
	m_current_cmp = initial;
	m_vamp_mode = false;
	m_mvmt_checklist[0] = false;
	m_mvmt_checklist[1] = false;
	m_mvmt_checklist[2] = false;
	m_mvmt_checklist[3] = false;
	m_vamp_quota = VAMP_KILLS_NEEDED;

}

void TutorialState::terminate() {
	if (m_background_music != nullptr)
		Mix_FreeMusic(m_background_music);
	if (m_player_dead_sound != nullptr)
		Mix_FreeChunk(m_player_dead_sound);
	if (m_player_eat_sound != nullptr)
		Mix_FreeChunk(m_player_eat_sound);
	if (m_player_explosion != nullptr)
		Mix_FreeChunk(m_player_explosion);

	m_player.destroy();
	for (auto& turtle : m_turtles)
		turtle.destroy();
	m_health.destroy();
	m_turtles.clear();
}

void TutorialState::update(GameEngine *game) {
	int w, h;
	glfwGetFramebufferSize(game->getM_window(), &w, &h);
	vec2 screen = { (float)w / game->getM_screen_scale(), (float)h / game->getM_screen_scale() };
	m_health.setHealth((int)m_player.get_health());

	// Checking Player - Turtle collisions
	auto turtle_it = m_turtles.begin();
	while (turtle_it != m_turtles.end())
	{
		if (m_player.collides_with(*turtle_it))
		{
			if (m_player.is_alive()) {
				if (m_player.get_iframes() <= 0.f) {
					m_player.set_iframes(500.f);
					lose_health(DAMAGE_ENEMY);
					m_turtles.erase(turtle_it);
				}
			}
			break;
		}
		else
		{
			turtle_it++;
		}
	}
	float turtle_limit = m_current_cmp == shooting ? 1 : m_current_cmp == vamp ? m_vamp_quota : 0;
	if (m_turtles.size() < turtle_limit)
	{
		if (!spawn_turtle())
			throw std::runtime_error("Failed spawn turtle");

		Turtle& new_turtle = m_turtles.back();

		// Setting random initial position
		if (m_current_cmp == vamp) {
			new_turtle.set_position({ 50 + m_dist(m_rng) * (screen.x - 100),  200 + m_dist(m_rng) * (screen.y - 600) });
			new_turtle.set_speed(0.f);
		}
		else {
			new_turtle.set_position({ 400.f,0.f });
		}
	}

	// Checking Player Bullet - Enemy collisions
	auto bullet_it = m_player.bullets.begin();
	while (bullet_it != m_player.bullets.end())
	{
		bool eraseBullet = false;
		auto turtle_it = m_turtles.begin();
		while (turtle_it != m_turtles.end())
		{
			if (bullet_it->collides_with(*turtle_it))
			{
				eraseBullet = true;
				turtle_it = m_turtles.erase(turtle_it);
				Mix_PlayChannel(-1, m_player_explosion, 0);
				++m_points;
				m_vamp_mode_charge++;
				if (m_current_cmp == shooting) {
					m_dialogue.next();
					m_current_cmp = vamp;
					m_vamp_mode_charge = 10;
				}
				break;
			}
			else {
				++turtle_it;
			}
		}
		if (eraseBullet)
			bullet_it = m_player.bullets.erase(bullet_it);
		else
			++bullet_it;
	}

	// check for vamp/turtle collisions
	if (m_vamp_mode) {
		turtle_it = m_turtles.begin();
		while (turtle_it != m_turtles.end()) {
			if (m_vamp.collides_with(*turtle_it)) {
				turtle_it = m_turtles.erase(turtle_it);
				add_health(VAMP_HEAL);
				m_vamp_quota--;
				continue;
			}

			++turtle_it;
		}
	}
	if (m_vamp_quota == 0 && m_current_cmp == vamp) {
		m_current_cmp = clear;
		m_dialogue.next();
	}

	// Updating all entities, making the turtle and fish
	// faster based on current
	float elapsed_ms = game->getElapsed_ms();
	m_player.update(elapsed_ms * m_current_speed, keyMap, mouse_position);
	m_vamp.update(elapsed_ms * m_current_speed, m_player.get_position());
	for (auto& turtle : m_turtles) {
		if (turtle.get_position().y > 400.f) turtle.set_speed(0.f);
		turtle.update(elapsed_ms * m_current_speed);
	}

	// for debugging purposes
	if (keyMap[GLFW_KEY_F]) {
		m_vamp_mode_charge = 10;
	}

	if (m_vamp_mode_charge >= 10 && keyMap[GLFW_KEY_ENTER]) {
		m_vamp_mode = true;
		m_vamp_mode_timer = VAMP_MODE_DURATION;
		m_vamp_mode_charge = 0;
		m_current_speed = 0.5f;

		m_vamp.init(m_player.get_position(), 0.785398f);
	}

	if (m_vamp_mode_timer > 0.f) {
		m_vamp_mode_timer -= elapsed_ms * m_current_speed;
		m_vamp.update(elapsed_ms, m_player.get_position());


		if (m_vamp_mode_timer <= 0.f) {
			m_current_speed = 1.f;
			m_vamp_mode = false;
			m_vamp.destroy();
		}
	}

	// Removing out of screen bullets
	// TODO move into player code? do same thing for boss/enemy bullets?
	bullet_it = m_player.bullets.begin();
	while (bullet_it != m_player.bullets.end()) {
		float h = bullet_it->get_bounding_box().y / 2;
		if (bullet_it->get_position().y + h < 0.f)
		{
			bullet_it = m_player.bullets.erase(bullet_it);
			continue;
		}

		++bullet_it;
	}

	// If salmon is dead, restart the game after the fading animation
	if (!m_player.is_alive() &&
		m_space.get_salmon_dead_time() > 5) {
		reset(screen);
	}
}

void TutorialState::draw(GameEngine *game) {
	// Clearing error buffer
	gl_flush_errors();

	// Getting size of window
	int w, h;
	GLFWwindow* m_window = game->getM_window();
	glfwGetFramebufferSize(m_window, &w, &h);

	// Updating window title with points
	std::stringstream title_ss;
	title_ss << "Points: " << m_points;
	glfwSetWindowTitle(m_window, title_ss.str().c_str());

	/////////////////////////////////////
	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, game->getM_frame_buffer());

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
	float right = (float)w / game->getM_screen_scale();// *0.5;
	float bottom = (float)h / game->getM_screen_scale();// *0.5;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	mat3 projection_2D{ { sx, 0.f, 0.f },{ 0.f, sy, 0.f },{ tx, ty, 1.f } };

	// Drawing entities
	for (auto& turtle : m_turtles)
		turtle.draw(projection_2D);
	if (m_vamp_mode) {
		m_vamp.draw(projection_2D);
	}
	m_player.draw(projection_2D);
	m_health.draw(projection_2D);


	/////////////////////
	// Truely render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(0, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, game->getM_screen_tex().id);

	m_space.draw(projection_2D);
	m_dialogue.draw(projection_2D);


	//////////////////
	// Presenting
	glfwSwapBuffers(m_window);
}

void TutorialState::lose_health(int damage) {
	m_player.lose_health(damage);
	Mix_PlayChannel(-1, m_player_dead_sound, 0);
	if (!m_player.is_alive()) {
		m_space.set_salmon_dead();
	}
}

void TutorialState::add_health(int heal) {
	float dif = MAX_HEALTH - m_player.get_health();

	int healVal = 0;
	if (dif >= heal)
		healVal = heal;
	else if (dif > 0)
		healVal = (int)dif;

	m_player.gain_health(healVal);
}

// Creates a new turtle and if successfull adds it to the list of turtles
bool TutorialState::spawn_turtle() {
	Turtle turtle;
	if (turtle.init())
	{
		m_turtles.emplace_back(turtle);
		return true;
	}
	fprintf(stderr, "Failed to spawn turtle");
	return false;
}

void TutorialState::on_key(GameEngine *game, GLFWwindow *wwindow, int key, int i, int action, int mod) {
	// Track which keys are being pressed or held
	(action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[key] = true : keyMap[key] = false;

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		// Get screen size
		int w, h;
		glfwGetFramebufferSize(game->getM_window(), &w, &h);
		vec2 screen = { (float)w / game->getM_screen_scale(), (float)h / game->getM_screen_scale() };
		reset(screen);
	}
	if (m_current_cmp == movement) {
		if (keyMap[GLFW_KEY_W]) m_mvmt_checklist[0] = true;
		if (keyMap[GLFW_KEY_S]) m_mvmt_checklist[1] = true;
		if (keyMap[GLFW_KEY_A]) m_mvmt_checklist[2] = true;
		if (keyMap[GLFW_KEY_D]) m_mvmt_checklist[3] = true;
		if (m_mvmt_checklist[0] && m_mvmt_checklist[1] && m_mvmt_checklist[2] && m_mvmt_checklist[3]) {
			m_current_cmp = shooting;
			m_dialogue.next();
		}
	}

	m_current_speed = fmax(0.f, m_current_speed);
}

void TutorialState::on_mouse_move(GameEngine *game, GLFWwindow *window, double xpos, double ypos) {
	mouse_position.x = xpos;
	mouse_position.y = ypos;
}

void TutorialState::on_mouse_button(GameEngine *game, GLFWwindow *window, int button, int action, int mods) {
	// Track which mouse buttons are being pressed or held
	(action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[button] = true : keyMap[button] = false;
	if (keyMap[button]) {
		if (m_current_cmp == initial) {
			m_current_cmp = movement;
			m_dialogue.next();
		}
		if (m_current_cmp == clear) {
			game->changeState(new LevelState());
		}
	}
}

void TutorialState::reset(vec2 screen) {
	m_player.destroy();
	m_vamp.destroy();
	m_player.init(screen, INIT_HEALTH);
	m_level_start = glfwGetTime();
	m_boss_mode = false;
	m_turtles.clear();
	Mix_PlayMusic(m_background_music, -1);
	m_mvmt_checklist[0] = false;
	m_mvmt_checklist[1] = false;
	m_mvmt_checklist[2] = false;
	m_mvmt_checklist[3] = false;
	m_current_cmp = initial;

	m_space.reset_salmon_dead_time();
	m_space.reset_boss_dead_time();
	m_current_speed = 1.f;
}
