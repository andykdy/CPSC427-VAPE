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
#include <Systems/EnemySpawnerSystem.hpp>
#include <Systems/MotionSystem.hpp>

#include "TutorialState.hpp"
#include "MainMenuState.hpp"

// Same as static in c, local to compilation unit
namespace
{
	const size_t VAMP_MODE_DURATION = 2000;
	const size_t MAX_HEALTH = 75;
	const size_t INIT_HEALTH = 50;
	const size_t DAMAGE_ENEMY = 5;
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

void TutorialState::init() {
	m_background_music = Mix_LoadMUS(audio_path("music_tutorial.wav"));
	m_player_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav"));
	m_player_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav"));
	m_player_explosion = Mix_LoadWAV(audio_path("explosion.wav"));
	m_player_charged = Mix_LoadWAV(audio_path("vamp_charge.wav"));

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

	// fprintf(stderr, "Loaded music\n");

	// Get screen size
	int w, h;
	glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
	vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

	GameEngine::getInstance().setM_current_speed(1.f);

	m_pause = &GameEngine::getInstance().getEntityManager()->addEntity<PauseMenu>();
	m_pause->init(screen);

	m_player = &GameEngine::getInstance().getEntityManager()->addEntity<Player>();
	m_player->init(screen, INIT_HEALTH);
	m_uiPanelBackground = &GameEngine::getInstance().getEntityManager()->addEntity<UIPanelBackground>();
	m_uiPanelBackground->init(screen, screen.y * 0.1f);
    m_uiPanel = &GameEngine::getInstance().getEntityManager()->addEntity<UIPanel>();
    m_uiPanel->init(screen, screen.y, screen.x);
    m_health = &GameEngine::getInstance().getEntityManager()->addEntity<Health>();
    m_health->init({53, screen.y-50});

    m_vamp_charge = &GameEngine::getInstance().getEntityManager()->addEntity<VampCharge>();
    m_vamp_charge->init({screen.x-52, screen.y-50});
	m_vamp_mode_charge = 0;

	GameEngine::getInstance().getSystemManager()->addSystem<MotionSystem>();

    m_space.set_position({screen.x/2, 0});

	m_space.init(textures_path("space_bg.png"));
	m_dialogue.init("TutorialText.png");
	m_continue_UI.init();
	m_current_cmp = initial;
	m_vamp_mode = false;
	m_mvmt_checklist[0] = false;
	m_mvmt_checklist[1] = false;
	m_mvmt_checklist[2] = false;
	m_mvmt_checklist[3] = false;
	m_vamp_quota = VAMP_KILLS_NEEDED;
	m_explosion.init();

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

	m_pause->destroy();

	m_player->destroy();
	m_vamp.destroy();
	for (auto& turtle : m_turtles)
		turtle->destroy();
    m_uiPanelBackground->destroy();
    m_uiPanel->destroy();
	m_health->destroy();
	m_vamp_charge->destroy();
	m_turtles.clear();
	m_dialogue.destroy();
	m_explosion.destroy();
	m_continue_UI.destroy();
	m_space.destroy();
}

void TutorialState::update(float ms) {
	if (m_pause->isPaused()) {
		m_pause->update(ms, mouse_position, keyMap);
		return;
	}

	int w, h;
	glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
	vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };
	m_health->setHealth(m_player->get_health());
	m_vamp_charge->setVampCharge(m_vamp_mode_charge);

	// Checking Player - Turtle collisions
	auto turtle_it = m_turtles.begin();
	while (turtle_it != m_turtles.end())
	{
		if (m_player->collides_with(**turtle_it))
		{
			if (m_player->is_alive()) {
				if (m_player->get_iframes() <= 0.f) {
					m_player->set_iframes(500.f);
					lose_health(DAMAGE_ENEMY);
                    m_explosion.spawn(m_player->get_position());
                    Mix_PlayChannel(-1, m_player_explosion, 0);
					(*turtle_it)->destroy();
					turtle_it = m_turtles.erase(turtle_it);
				}
			}
			break;
		}
		else
		{
			turtle_it++;
		}
	}
	float turtle_limit = m_current_cmp == shooting ? 1 : m_current_cmp == vamp_1 || m_current_cmp == vamp_2 ? m_vamp_quota : 0;
	if (m_turtles.size() < turtle_limit)
	{
		if (!spawn_turtle())
			throw std::runtime_error("Failed spawn turtle");

		Turtle* new_turtle = m_turtles.back();

		// Setting random initial position
		if (m_current_cmp == vamp_1 || m_current_cmp == vamp_2) {
			new_turtle->set_position({ 50 + m_dist(m_rng) * (screen.x - 100),  200 + m_dist(m_rng) * (screen.y - 600) });
			new_turtle->set_velocity({0.f, 0.f});
		}
		else {
			new_turtle->set_position({ 400.f,0.f });
		}
	}

	auto& playerBullets = m_player->projectiles;

	// Checking Player Bullet - Enemy collisions
	auto bullet_it = playerBullets.begin();
	while (bullet_it != playerBullets.end())
	{
		bool eraseBullet = false;
		auto turtle_it = m_turtles.begin();
		while (turtle_it != m_turtles.end())
		{
			if ((*bullet_it)->collides_with(**turtle_it))
			{
				eraseBullet = true;
                m_explosion.spawn((*turtle_it)->get_position());
				(*turtle_it)->destroy();
				turtle_it = m_turtles.erase(turtle_it);
				Mix_PlayChannel(-1, m_player_explosion, 0);
				++m_points;
				add_vamp_charge();
				if (m_current_cmp == shooting) {
					m_dialogue.next();
					m_current_cmp = vamp_1;
					m_vamp_mode_charge = 12;
				}
				break;
			}
			else {
				++turtle_it;
			}
		}
		if (eraseBullet) {
            (*bullet_it)->destroy();
            bullet_it = playerBullets.erase(bullet_it);
        }else
			++bullet_it;
	}

	// check for vamp/turtle collisions
	if (m_vamp_mode) {
		turtle_it = m_turtles.begin();
		while (turtle_it != m_turtles.end()) {
			if (m_vamp.collides_with(**turtle_it)) {
                m_explosion.spawn((*turtle_it)->get_position());
                Mix_PlayChannel(-1, m_player_explosion, 0);
				(*turtle_it)->destroy();
				turtle_it = m_turtles.erase(turtle_it);
				add_health(VAMP_HEAL);
				m_vamp_quota--;
				continue;
			}
			++turtle_it;
		}
	}
	if (m_vamp_quota == 0 && m_current_cmp == vamp_2 && !m_vamp_mode) {
		m_current_cmp = clear;
		m_dialogue.next();
		m_continue_UI.set_activity(true);
	}

    m_space.update(ms);
    m_explosion.update(ms);
	// Updating all entities, making the turtle and fish
	// faster based on current
	m_player->update(ms, keyMap, mouse_position);
	for (auto& turtle : m_turtles) {
		if (turtle->get_position().y > 400.f) turtle->set_velocity({0.f,0.f});
		turtle->update(ms);
	}

	// for debugging purposes
	if (keyMap[GLFW_KEY_F]) {
		m_vamp_mode_charge = 15;
	}
	if (m_current_cmp == vamp_2) {
		m_vamp_mode_charge = 15;
	}

	if (m_vamp_mode_charge == 15 && m_current_cmp == vamp_1) {
		m_current_cmp = vamp_2;
 		m_dialogue.next();
	}

	if (m_vamp_mode_charge >= 15 && keyMap[GLFW_KEY_ENTER]) {
		m_vamp_mode = true;
		m_vamp_mode_timer = VAMP_MODE_DURATION;
		m_vamp_mode_charge = 0;
		GameEngine::getInstance().setM_current_speed(0.5f);

		m_vamp.init(m_player->get_position());
	}

	if (m_vamp_mode_timer > 0.f) {
		m_vamp_mode_timer -= ms;
		m_vamp.update(ms, m_player->get_position());


		if (m_vamp_mode_timer <= 0.f) {
			GameEngine::getInstance().setM_current_speed(1.f);
			m_vamp_mode = false;
			m_vamp.destroy();
		}
	}

	// If salmon is dead, restart the game after the fading animation
	if (!m_player->is_alive() &&
		m_space.get_salmon_dead_time() > 5) {
		reset();
	}
}

void TutorialState::draw() {
	// Clearing error buffer
	gl_flush_errors();

	// Getting size of window
	int w, h;
	GLFWwindow* m_window = GameEngine::getInstance().getM_window();
	glfwGetFramebufferSize(m_window, &w, &h);

	// Updating window title with points
	std::stringstream title_ss;
	title_ss  << "Points: " << m_points;
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

	m_space.draw(projection_2D);
	// Drawing entities
	for (auto& turtle : m_turtles)
		turtle->draw(projection_2D);
	if (m_vamp_mode) {
		m_vamp.draw(projection_2D);
	}
	m_player->draw(projection_2D);
	m_uiPanelBackground->draw(projection_2D);
	m_health->draw(projection_2D);
	m_vamp_charge->draw(projection_2D);
	if (m_continue_UI.isActive()) {
		m_continue_UI.draw(projection_2D);
	}
    m_uiPanel->draw(projection_2D);
	m_dialogue.draw(projection_2D);
    m_explosion.draw(projection_2D);



	m_pause->draw(projection_2D);
	//////////////////
	// Presenting
	glfwSwapBuffers(m_window);
}

void TutorialState::lose_health(int damage) {
	m_player->lose_health(damage);
	Mix_PlayChannel(-1, m_player_dead_sound, 0);
	if (!m_player->is_alive()) {
		m_space.set_salmon_dead();
	}
}

void TutorialState::add_health(int heal) {
	float dif = MAX_HEALTH - m_player->get_health();

	int healVal = 0;
	if (dif >= heal)
		healVal = heal;
	else if (dif > 0)
		healVal = (int)dif;

	m_player->gain_health(healVal);
}

void TutorialState::add_vamp_charge() {
	if (m_vamp_mode_charge < 15) {
		m_vamp_mode_charge++;

		if (m_vamp_mode_charge == 15) {
			// TODO - replace with more appropriate sound
			Mix_PlayChannel(-1, m_player_charged, 0);
		}
	}
}


// Creates a new turtle and if successfull adds it to the list of turtles
bool TutorialState::spawn_turtle() {
	Turtle* turtle = &GameEngine::getInstance().getEntityManager()->addEntity<Turtle>();
	if (turtle->init())
	{
		m_turtles.emplace_back(turtle);
		return true;
	}
	fprintf(stderr, "Failed to spawn turtle");
	return false;
}

void TutorialState::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
	// Track which keys are being pressed or held
	(action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[key] = true : keyMap[key] = false;

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		return reset();
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
	if (keyMap[GLFW_KEY_LEFT_SHIFT] || keyMap[GLFW_KEY_RIGHT_SHIFT]) {
		if (m_current_cmp == initial) {
			m_current_cmp = movement;
			m_dialogue.next();
			m_continue_UI.set_activity(false);
		}
		if (m_current_cmp == clear) {
			GameEngine::getInstance().changeState(new MainMenuState());
		}
	}

	if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE)
	{
		return m_pause->toggle();
	}


	m_pause->on_key(wwindow, key, i, action, mod);
}

void TutorialState::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
	mouse_position.x = xpos;
	mouse_position.y = ypos;
}

void TutorialState::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
	// Track which mouse buttons are being pressed or held
	(action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[button] = true : keyMap[button] = false;
}

void TutorialState::reset() {
	GameEngine::getInstance().changeState(new TutorialState());
}
