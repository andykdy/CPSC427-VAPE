/*!
 * @author: Cody Newman
 *
 * Created on: 9/17/2019
 */

// stlib
#include <string.h>
#include <cassert>
#include <sstream>

#include "Level.hpp"

// Same as static in c, local to compilation unit
namespace
{
    const size_t MAX_TURTLES = 15;
    const size_t MAX_FISH = 5;
    const size_t TURTLE_DELAY_MS = 2000;
    const size_t FISH_DELAY_MS = 5000;
    const size_t BULLET_COOLDOWN_MS = 1000;
}


Level::Level() :
m_points(0),
m_next_turtle_spawn(0.f),
m_next_fish_spawn(0.f)
{
    // Seeding rng with random device
    m_rng = std::default_random_engine(std::random_device()());
}

void Level::init(GameEngine *game) {
    m_background_music = Mix_LoadMUS(audio_path("music.wav"));
    m_salmon_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav"));
    m_salmon_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav"));

    if (m_background_music == nullptr || m_salmon_dead_sound == nullptr || m_salmon_eat_sound == nullptr)
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

    m_salmon.init(screen);
    m_water.init();
    m_pebbles_emitter.init();
}

void Level::terminate() {
    if (m_background_music != nullptr)
        Mix_FreeMusic(m_background_music);
    if (m_salmon_dead_sound != nullptr)
        Mix_FreeChunk(m_salmon_dead_sound);
    if (m_salmon_eat_sound != nullptr)
        Mix_FreeChunk(m_salmon_eat_sound);

    m_salmon.destroy();
    m_pebbles_emitter.destroy();
    for (auto& turtle : m_turtles)
        turtle.destroy();
    for (auto& fish : m_fish)
        fish.destroy();
    for (auto& bullet : m_bullets)
        bullet.destroy();
    m_turtles.clear();
    m_fish.clear();
    m_bullets.clear();
}

void Level::update(GameEngine *game) {
    int w, h;
    glfwGetFramebufferSize(game->getM_window(), &w, &h);
    vec2 screen = { (float)w / game->getM_screen_scale(), (float)h / game->getM_screen_scale() };

    // Checking Salmon - Turtle collisions
    for (const auto& turtle : m_turtles)
    {
        if (m_salmon.collides_with(turtle))
        {
            if (m_salmon.is_alive()) {
                Mix_PlayChannel(-1, m_salmon_dead_sound, 0);
                m_water.set_salmon_dead();
            }
            m_salmon.kill();
            break;
        }
    }

    // Checking Salmon - Fish collisions
    auto fish_it = m_fish.begin();
    while (fish_it != m_fish.end())
    {
        if (m_salmon.is_alive() && m_salmon.collides_with(*fish_it))
        {
            fish_it = m_fish.erase(fish_it);
            m_salmon.light_up();
            Mix_PlayChannel(-1, m_salmon_eat_sound, 0);
            ++m_points;
        }
        else
            ++fish_it;
    }

    // Checking Bullet - Turtle collisions
    auto bullet_it = m_bullets.begin();
    while (bullet_it != m_bullets.end())
    {
        bool eraseBullet = false;
        auto turtle_it = m_turtles.begin();
        while (turtle_it != m_turtles.end())
        {
            if (bullet_it->collides_with(*turtle_it))
            {
                // TODO sound?
                eraseBullet = true;
                turtle_it = m_turtles.erase(turtle_it);
                break;
            } else {
                ++turtle_it;
            }
        }
        if (eraseBullet)
            bullet_it = m_bullets.erase(bullet_it);
        else
            ++bullet_it;
    }

    // Updating all entities, making the turtle and fish
    // faster based on current
    float elapsed_ms = game->getElapsed_ms();
    m_salmon.update(elapsed_ms, keyMap, mouse_position);
    for (auto& turtle : m_turtles)
        turtle.update(elapsed_ms * m_current_speed);
    for (auto& fish : m_fish)
        fish.update(elapsed_ms * m_current_speed);
    for (auto& bullet : m_bullets)
        bullet.update(elapsed_ms*m_current_speed);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // HANDLE PEBBLE SPAWN/UPDATES HERE
    // DON'T WORRY ABOUT THIS UNTIL ASSIGNMENT 3
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Removing out of screen turtles
    auto turtle_it = m_turtles.begin();
    while (turtle_it != m_turtles.end())
    {
        float w = turtle_it->get_bounding_box().x / 2;
        if (turtle_it->get_position().x + w < 0.f)
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

    // Removing out of screen bullets
    bullet_it = m_bullets.begin();
    while(bullet_it != m_bullets.end()) {
        float w = bullet_it->get_bounding_box().x / 2;
        if (bullet_it->get_position().x + w < 0.f)
        {
            bullet_it = m_bullets.erase(bullet_it);
            continue;
        }

        ++bullet_it;
    }

    // Spawning new turtles
    m_next_turtle_spawn -= elapsed_ms * m_current_speed;
    if (m_turtles.size() <= MAX_TURTLES && m_next_turtle_spawn < 0.f)
    {
        if (!spawn_turtle())
            throw std::runtime_error("Failed spawn turtle");

        Turtle& new_turtle = m_turtles.back();

        // Setting random initial position
        new_turtle.set_position({ 50 + m_dist(m_rng) * (screen.x - 100), -150 });

        // Next spawn
        m_next_turtle_spawn = (TURTLE_DELAY_MS / 2) + m_dist(m_rng) * (TURTLE_DELAY_MS/2);
    }

    // Spawning new fish
    /*
	m_next_fish_spawn -= elapsed_ms * m_current_speed;
	if (m_fish.size() <= MAX_FISH && m_next_fish_spawn < 0.f)
	{
		if (!spawn_fish())
			return false;
		Fish& new_fish = m_fish.back();

		new_fish.set_position({ screen.x + 150, 50 + m_dist(m_rng) *  (screen.y - 100) });

		m_next_fish_spawn = (FISH_DELAY_MS / 2) + m_dist(m_rng) * (FISH_DELAY_MS / 2);
	}
     */

    // Spawning bullets
    m_bullet_cooldown -= elapsed_ms * m_current_speed;
    if (keyMap[GLFW_KEY_RIGHT_CONTROL] && m_bullet_cooldown < 0.f) {
        if (!spawn_bullet())
            throw std::runtime_error("Failed to spawn bullet");
        Bullet& new_bullet = m_bullets.back();
        m_bullet_cooldown = BULLET_COOLDOWN_MS;
    }

    // If salmon is dead, restart the game after the fading animation
    if (!m_salmon.is_alive() &&
        m_water.get_salmon_dead_time() > 5) {
        m_salmon.destroy();
        m_salmon.init(screen);
        m_pebbles_emitter.destroy();
        m_pebbles_emitter.init();
        m_turtles.clear();
        m_fish.clear();
        m_bullets.clear();
        m_water.reset_salmon_dead_time();
        m_current_speed = 1.f;
    }
}

void Level::draw(GameEngine *game) {
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
    const float clear_color[3] = { 0.3f, 0.3f, 0.8f };
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
    for (auto& bullet : m_bullets)
        bullet.draw(projection_2D);
    m_salmon.draw(projection_2D);

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

    m_water.draw(projection_2D);

    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);
}

// Creates a new turtle and if successfull adds it to the list of turtles
bool Level::spawn_turtle() {
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
bool Level::spawn_fish() {
    Fish fish;
    if (fish.init())
    {
        m_fish.emplace_back(fish);
        return true;
    }
    fprintf(stderr, "Failed to spawn fish");
    return false;
}

bool Level::spawn_bullet() {
    vec2 position = m_salmon.get_position();

    Bullet bullet;
    if (bullet.init(position, mouse_position)) {
        m_bullets.emplace_back(bullet);
        return true;
    }
    fprintf(stderr, "Failed to spawn bullet");
    return false;
}

void Level::on_key(GameEngine *game, GLFWwindow *wwindow, int key, int i, int action, int mod) {
    // Track which keys are being pressed or held
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[key] = true : keyMap[key] = false;

    // Resetting game
    if (action == GLFW_RELEASE && key == GLFW_KEY_R)
    {
        // Get screen size
        int w, h;
        glfwGetFramebufferSize(game->getM_window(), &w, &h);
        vec2 screen = { (float)w / game->getM_screen_scale(), (float)h / game->getM_screen_scale() };
        m_salmon.destroy();
        m_salmon.init(screen);
        m_pebbles_emitter.destroy();
        m_pebbles_emitter.init();
        m_turtles.clear();
        m_fish.clear();
        m_water.reset_salmon_dead_time();
        m_current_speed = 1.f;
    }

    // Control the current speed with `<` `>`
    if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) &&  key == GLFW_KEY_COMMA)
        m_current_speed -= 0.1f;
    if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD)
        m_current_speed += 0.1f;

    m_current_speed = fmax(0.f, m_current_speed);
}

void Level::on_mouse_move(GameEngine *game, GLFWwindow *window, double xpos, double ypos) {
    mouse_position.x = xpos;
    mouse_position.y = ypos;
}

void Level::on_mouse_button(GameEngine *game, GLFWwindow *window, int button, int action, int mods) {
    // Track which mouse buttons are being pressed or held
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[button] = true : keyMap[button] = false;
}
