/*!
 * @author: Cody Newman
 *
 * Created on: 9/17/2019
 */

// stlib
#include <string.h>
#include <cassert>
#include <sstream>

#include "LevelState.hpp"
#include "MainMenuState.hpp"

// Same as static in c, local to compilation unit
namespace
{
    const size_t MAX_TURTLES = 30;
    const size_t MAX_FISH = 5;
    const size_t TURTLE_DELAY_MS = 500;
    const size_t FISH_DELAY_MS = 5000;
    const size_t INIT_HEALTH = 50;
    const size_t DAMAGE_ENEMY = 5;
    const size_t DAMAGE_BOSS = 15;
    const size_t BOSS_TIME = 30;
    const size_t VAMP_HEAL = 2;
}


LevelState::LevelState() :
m_points(0),
m_next_turtle_spawn(0.f),
m_next_fish_spawn(0.f)
{
    // Seeding rng with random device
    m_rng = std::default_random_engine(std::random_device()());
}

void LevelState::init(GameEngine *game) {
    m_background_music = Mix_LoadMUS(audio_path("music.wav"));
    m_boss_music = Mix_LoadMUS(audio_path("music_boss1.wav"));
    m_victory_music = Mix_LoadMUS(audio_path("music_victory.wav"));
    m_player_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav"));
    m_player_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav"));
    m_player_explosion = Mix_LoadWAV(audio_path("explosion.wav"));

    if (m_background_music == nullptr || m_boss_music == nullptr || m_victory_music == nullptr ||
        m_player_dead_sound == nullptr || m_player_eat_sound == nullptr)
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
    m_boss_mode = false;
    m_spawn_enemies = true;

    m_player.init(screen, INIT_HEALTH);
    m_space.init();

    init_health();
}

void LevelState::terminate() {
    if (m_background_music != nullptr)
        Mix_FreeMusic(m_background_music);
    if (m_boss_music != nullptr)
        Mix_FreeMusic(m_boss_music);
    if (m_victory_music != nullptr)
        Mix_FreeMusic(m_victory_music);
    if (m_player_dead_sound != nullptr)
        Mix_FreeChunk(m_player_dead_sound);
    if (m_player_eat_sound != nullptr)
        Mix_FreeChunk(m_player_eat_sound);
    if (m_player_explosion != nullptr)
        Mix_FreeChunk(m_player_explosion);

    m_player.destroy();
    for (auto& turtle : m_turtles)
        turtle.destroy();
    for (auto& fish : m_fish)
        fish.destroy();
    for(auto& health: m_health)
        health.destroy();
    m_turtles.clear();
    m_fish.clear();
    m_health.clear();
}

void LevelState::update(GameEngine *game) {
    int w, h;
    glfwGetFramebufferSize(game->getM_window(), &w, &h);
    vec2 screen = { (float)w / game->getM_screen_scale(), (float)h / game->getM_screen_scale() };

    // To prepare for the boss, stop spawning enemies and change the music
    if (glfwGetTime() - m_level_start >= BOSS_TIME - 4 && m_spawn_enemies) {
        m_spawn_enemies = false;
        Mix_PlayMusic(m_boss_music, -1);
    }
    // Spawn the boss
    if (glfwGetTime() - m_level_start >= BOSS_TIME && !m_boss_mode) {
        m_boss_mode = true;
        m_boss.init(game);
        m_boss.set_position({static_cast<float>(w/2), static_cast<float>(h/10)});
    }

    // Checking Player - Turtle collisions
	auto turtle_it = m_turtles.begin();
	while (turtle_it != m_turtles.end())
	{
        if (m_player.collides_with(*turtle_it))
        {
            if (m_player.is_alive()) {
				if (m_player.get_iframes() <= 0.f) {
					m_player.set_iframes(500.f);
					lose_health();
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

    // Checking Player - Fish collisions
    auto fish_it = m_fish.begin();
    while (fish_it != m_fish.end())
    {
        if (m_player.is_alive() && m_player.collides_with(*fish_it))
        {
            fish_it = m_fish.erase(fish_it);
            m_player.light_up();
            Mix_PlayChannel(-1, m_player_eat_sound, 0);
            ++m_points;
        }
        else
            ++fish_it;
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
                // TODO sound
                Mix_PlayChannel(-1,m_player_explosion,0);
                ++m_points;
                break;
            } else {
                ++turtle_it;
            }
        }
        if (m_boss_mode && bullet_it->collides_with(m_boss)) {
            eraseBullet = true;
            // TODO sound
            m_boss.addDamage(2);
        }
        if (eraseBullet)
            bullet_it = m_player.bullets.erase(bullet_it);
        else
            ++bullet_it;
    }

    // Updating all entities, making the turtle and fish
    // faster based on current
    float elapsed_ms = game->getElapsed_ms();
    m_player.update(elapsed_ms, keyMap, mouse_position);
    for (auto& turtle : m_turtles)
        turtle.update(elapsed_ms * m_current_speed);
    for (auto& fish : m_fish)
        fish.update(elapsed_ms * m_current_speed);

    // Removing out of screen turtles
    turtle_it = m_turtles.begin();
    while (turtle_it != m_turtles.end())
    {
        float h = turtle_it->get_bounding_box().y / 2;
        if (turtle_it->get_position().y - h > screen.y)
        {
            turtle_it = m_turtles.erase(turtle_it);
            continue;
        }

        ++turtle_it;
    }

    // Removing out of screen fish
    fish_it = m_fish.begin();
    while (fish_it != m_fish.end())
    {
        float w = fish_it->get_bounding_box().x / 2;
        if (fish_it->get_position().x + w < 0.f)
        {
            fish_it = m_fish.erase(fish_it);
            continue;
        }

        ++fish_it;
    }

    // Spawning new turtles
    m_next_turtle_spawn -= elapsed_ms * m_current_speed;
    if (m_spawn_enemies && m_turtles.size() <= MAX_TURTLES && m_next_turtle_spawn < 0.f)
    {
        if (!spawn_turtle())
            throw std::runtime_error("Failed spawn turtle");

        Turtle& new_turtle = m_turtles.back();

        // Setting random initial position
        new_turtle.set_position({ 50 + m_dist(m_rng) * (screen.x - 100), -150 });

        // Next spawn
        m_next_turtle_spawn = (TURTLE_DELAY_MS / 2) + m_dist(m_rng) * (TURTLE_DELAY_MS/2);
    }

    // Removing out of screen bullets
    // TODO move into player code? do same thing for boss/enemy bullets?
    bullet_it = m_player.bullets.begin();
    while(bullet_it != m_player.bullets.end()) {
        float h = bullet_it->get_bounding_box().y / 2;
        if (bullet_it->get_position().y + h < 0.f)
        {
            bullet_it = m_player.bullets.erase(bullet_it);
            continue;
        }

        ++bullet_it;
    }


    // Boss specific code
    if (m_boss_mode) {
        // If boss drops below 0 health, set him as killed, award points, start timer
        if (m_boss.is_alive() && m_boss.getHealth() <= 0) {
            Mix_PlayMusic(m_victory_music, 0);
            m_boss.kill();
            m_points += 100;
            m_space.set_boss_dead();
        }

        // Checking Enemy Bullet - Player collisions
        auto boss_bullet_it = m_boss.bullets.begin();
        while (boss_bullet_it != m_boss.bullets.end()) {
            if (boss_bullet_it->collides_with(m_player))
            {
                boss_bullet_it = m_boss.bullets.erase(boss_bullet_it);
                if (m_player.is_alive() && m_player.get_iframes() <= 0.f) {
                    m_player.set_iframes(500.f);
                    lose_health();
                }
                break;
            } else {
                ++boss_bullet_it;
            }
        }

        // Removing out of screen bullets
        // TODO move into boss class?
        auto bullet_it = m_boss.bullets.begin();
        while(bullet_it != m_boss.bullets.end()) {
            float h = bullet_it->get_bounding_box().y / 2;
            if (bullet_it->get_position().y - h > screen.y)
            {
                bullet_it = m_boss.bullets.erase(bullet_it);
                continue;
            } else {
                ++bullet_it;
            }
        }

        m_boss.update(elapsed_ms);

        // If boss dies, return to main menu
        if (m_boss.getHealth() <= 0 && m_space.get_boss_dead_time() > 5)
        {
            game->changeState(new MainMenuState());
        }
    }

    // If salmon is dead, restart the game after the fading animation
    if (!m_player.is_alive() &&
        m_space.get_salmon_dead_time() > 5) {
        m_player.destroy();
        m_player.init(screen, INIT_HEALTH);
        m_boss.destroy();
        m_level_start = glfwGetTime();
        m_boss_mode = false;
        m_spawn_enemies = true;
        m_turtles.clear();
        m_fish.clear();
        m_health.clear();
        init_health();
        Mix_PlayMusic(m_background_music, -1);

        m_space.reset_salmon_dead_time();
        m_space.reset_boss_dead_time();
        m_current_speed = 1.f;
    }
}

void LevelState::draw(GameEngine *game) {
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

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // DRAW DEBUG INFO HERE
    // DON'T WORRY ABOUT THIS UNTIL ASSIGNMENT 2
    // You will want to create your own data structures for passing in
    // relevant information to your debug draw call.
    // The shaders coloured.vs.glsl and coloured.fs.glsl should be helpful.
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Drawing entities
    for (auto& turtle : m_turtles)
        turtle.draw(projection_2D);
    for (auto& fish : m_fish)
        fish.draw(projection_2D);
    m_player.draw(projection_2D);
    if (m_boss_mode) {
        m_boss.draw(projection_2D);
    }
    for(auto& health : m_health)
        health.draw(projection_2D);

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

    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);
}

void LevelState::init_health() {
    for(int i = 0; i < INIT_HEALTH; i++) {
        Health health;

        if(health.init( {(45 + ((float)(INIT_HEALTH - i - 1) * 5)), 60})) {
            m_health.emplace_back(health);
        } else {
            throw std::runtime_error("Failed to init health");
        }
    }
}

void LevelState::lose_health() {
    m_player.lose_health(DAMAGE_ENEMY);
    auto health_it = m_health.begin();
    for (int i = 0; i < DAMAGE_ENEMY; i++) {
        m_health.erase(health_it);
    }
    Mix_PlayChannel(-1, m_player_dead_sound, 0);
    if (!m_player.is_alive()) {
        m_space.set_salmon_dead();
    }
}

// Added in preparation for vamp mode
void LevelState::add_health() {
    // m_player.gain_health(VAMP_HEAL); Added by vamp branch
    auto i = m_health.size();
    Health health;
    if(health.init( {(45 + ((float)(INIT_HEALTH - i - 1) * 5)), 60})) {
        m_health.emplace_back(health);
    } else {
        throw std::runtime_error("Failed to add health");
    }
}

// Creates a new turtle and if successfull adds it to the list of turtles
bool LevelState::spawn_turtle() {
    Turtle turtle;
    if (turtle.init())
    {
        m_turtles.emplace_back(turtle);
        return true;
    }
    fprintf(stderr, "Failed to spawn turtle");
    return false;
}

// Creates a new fish and if successfull adds it to the list of fish
bool LevelState::spawn_fish() {
    Fish fish;
    if (fish.init())
    {
        m_fish.emplace_back(fish);
        return true;
    }
    fprintf(stderr, "Failed to spawn fish");
    return false;
}

void LevelState::on_key(GameEngine *game, GLFWwindow *wwindow, int key, int i, int action, int mod) {
    // Track which keys are being pressed or held
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[key] = true : keyMap[key] = false;

    // Resetting game
    if (action == GLFW_RELEASE && key == GLFW_KEY_R)
    {
        // Get screen size
        int w, h;
        glfwGetFramebufferSize(game->getM_window(), &w, &h);
        vec2 screen = { (float)w / game->getM_screen_scale(), (float)h / game->getM_screen_scale() };
        m_player.destroy();
        m_player.init(screen, INIT_HEALTH);
        m_turtles.clear();
        m_fish.clear();
        m_space.reset_salmon_dead_time();
        m_space.reset_boss_dead_time();
        m_current_speed = 1.f;

        init_health();
    }

    // Control the current speed with `<` `>`
    if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) &&  key == GLFW_KEY_COMMA)
        m_current_speed -= 0.1f;
    if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD)
        m_current_speed += 0.1f;

    m_current_speed = fmax(0.f, m_current_speed);
}

void LevelState::on_mouse_move(GameEngine *game, GLFWwindow *window, double xpos, double ypos) {
    mouse_position.x = xpos;
    mouse_position.y = ypos;
}

void LevelState::on_mouse_button(GameEngine *game, GLFWwindow *window, int button, int action, int mods) {
    // Track which mouse buttons are being pressed or held
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[button] = true : keyMap[button] = false;
}