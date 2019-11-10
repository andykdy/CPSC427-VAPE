/*!
 * @author: Cody Newman
 *
 * Created on: 9/17/2019
 */

// stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <Systems/MotionSystem.hpp>
#include <Systems/EnemySpawnerSystem.hpp>
#include <iostream>
#include <Systems/CollisionSystem.hpp>
#include <Entities/Bosses/Boss2.hpp>

#include "LevelState.hpp"
#include "MainMenuState.hpp"

// Same as static in c, local to compilation unit
namespace
{
    const size_t MAX_TURTLES = 30;
    const size_t MAX_FISH = 5;
    const size_t TURTLE_DELAY_MS = 500;
    const size_t FISH_DELAY_MS = 5000;
    const size_t VAMP_MODE_DURATION = 1500;
    const size_t MAX_HEALTH = 75;
    const size_t INIT_HEALTH = 50;
    const size_t DAMAGE_ENEMY = 5;
    const size_t DAMAGE_BOSS = 5;
    const size_t VAMP_HEAL = 2;
}


LevelState::LevelState(Levels::Level level, unsigned int points) :
m_level(level),
m_points(points)
{
    // Seeding rng with random device
    m_rng = std::default_random_engine(std::random_device()());
}

void LevelState::init() {
    //std::cout << "init" << std::endl;
    m_background_music = Mix_LoadMUS(m_level.backgroundMusic);
    m_boss_music = Mix_LoadMUS(m_level.bossMusic);
    m_victory_music = Mix_LoadMUS(audio_path("music_victory.wav"));
    m_player_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav"));
    m_player_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav"));
    m_player_explosion = Mix_LoadWAV(audio_path("explosion.wav"));
    m_player_charged = Mix_LoadWAV(audio_path("vamp_charge.wav"));


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
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

    m_vamp_mode = false;
    m_level_time = 0;
    m_boss_mode = false;
    m_boss_pre = false;

    m_player = &GameEngine::getInstance().getEntityManager()->addEntity<Player>();
    m_player->init(screen, INIT_HEALTH);
    m_uiPanel = &GameEngine::getInstance().getEntityManager()->addEntity<UIPanel>();
    m_uiPanel->init(screen, screen.y * 0.1f);
    m_health = &GameEngine::getInstance().getEntityManager()->addEntity<Health>();
    m_health->init({45, screen.y-50});
    m_vamp_charge = &GameEngine::getInstance().getEntityManager()->addEntity<VampCharge>();
    m_vamp_charge->init({screen.x/2.f, screen.y - (screen.y/12.f)});
    m_vamp_mode_charge = 0;
	m_dialogue.init(m_level.bossDialogue);
	m_dialogue.deactivate();
    m_space.init(m_level.backgroundTexture);
    m_explosion.init();
    m_boss = m_level.spawnBoss(GameEngine::getInstance().getEntityManager());
    m_boss->init(screen);

    m_space.set_position({screen.x/2, 0});

    GameEngine::getInstance().getSystemManager()->addSystem<MotionSystem>();
    // GameEngine::getInstance().getSystemManager()->addSystem<CollisionSystem>();
    auto & spawn = GameEngine::getInstance().getSystemManager()->addSystem<EnemySpawnerSystem>();
    spawn.reset(m_level.timeline);
    m_turtles = spawn.getEnemies(); // TODO, probably just get rid of m_turtles, pull from spawn system when needed
    GameEngine::getInstance().setM_current_speed(1.f);
    //std::cout << "initEnd" << std::endl;
}

void LevelState::terminate() {
    // TODO system cleanup
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

    m_player->destroy();
    for (auto& turtle : *m_turtles){ // TODO do in enemyspawnersystem
        turtle->destroy();
    }

    m_uiPanel->destroy();
    m_health->destroy();
    m_vamp_charge->destroy();
    m_turtles->clear();
    if (m_boss != nullptr)
        m_boss->destroy();
	m_dialogue.destroy();
	m_explosion.destroy();
	m_space.destroy();
}

void LevelState::update(float ms) {
    //std::cout << "update" << std::endl;
    int w, h;
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

    m_level_time += ms;

    // To prepare for the boss, stop spawning enemies and change the music
    if (m_level_time >= m_level.bossTime - 5000 && !m_boss_pre) {
        Mix_PlayMusic(m_boss_music, -1);
		m_dialogue.activate();
        m_boss_pre = true;
    }
    // Spawn the boss
    if (m_level_time >= m_level.bossTime && !m_boss_mode) {
		m_dialogue.deactivate();
        m_boss_mode = true;
        m_boss->set_position({static_cast<float>(w/2), static_cast<float>(h/10)});
    }

    m_health->setHealth(m_player->get_health());
    m_vamp_charge->setVampCharge(m_vamp_mode_charge);

    // Checking Player - Turtle collisions
	auto turtle_it = m_turtles->begin();
	while (turtle_it != m_turtles->end())
	{
        if (m_player->collides_with(**turtle_it))
        {
            if (m_player->is_alive()) {
				if (m_player->get_iframes() <= 0.f) {
					m_player->set_iframes(500.f);
					lose_health(DAMAGE_ENEMY);
                    Mix_PlayChannel(-1, m_player_explosion, 0);
					(*turtle_it)->destroy();
                    turtle_it = m_turtles->erase(turtle_it);
				}
			}
            break;
		} else
		{
			turtle_it++;
		}
    }

    // Checking Player Bullet - Enemy collisions
    auto& playerBullets = m_player->bullets;
    auto bullet_it = playerBullets.begin();
    while (bullet_it != playerBullets.end())
    {
        bool eraseBullet = false;
        auto turtle_it = m_turtles->begin();
        while (turtle_it != m_turtles->end())
        {
            if ((*bullet_it)->collides_with(**turtle_it))
            {
                eraseBullet = true;
                m_explosion.spawn((*turtle_it)->get_position());
                (*turtle_it)->destroy();
                turtle_it = m_turtles->erase(turtle_it);
                Mix_PlayChannel(-1,m_player_explosion,0);
                // expl

                ++m_points;
                add_vamp_charge();

                break;
            } else {
                ++turtle_it;
            }
        }
        if (m_boss_mode && (*bullet_it)->collides_with(*m_boss)) {
            eraseBullet = true;
            // TODO sound
            m_boss->addDamage(2);
        }
        if (eraseBullet) {
            (*bullet_it)->destroy();
            bullet_it = playerBullets.erase(bullet_it);
        } else
            ++bullet_it;
    }

    // check for vamp/turtle collisions
    if (m_vamp_mode) {
        turtle_it = m_turtles->begin();
        while (turtle_it != m_turtles->end()) {
            if (m_vamp.collides_with(**turtle_it)) {
                m_explosion.spawn((*turtle_it)->get_position());
                Mix_PlayChannel(-1, m_player_explosion, 0);
                (*turtle_it)->destroy();
                turtle_it = m_turtles->erase(turtle_it);
                add_health(VAMP_HEAL);

                continue;
            }

            ++turtle_it;
        }
    }

    m_space.update(ms);
    m_explosion.update(ms);

    // Updating all entities, making the turtle and fish
    // faster based on current
    m_player->update(ms, keyMap, mouse_position);
    for (auto& turtle : *m_turtles)
        turtle->update(ms);

    // Removing out of screen turtles
    turtle_it = m_turtles->begin();
    while (turtle_it != m_turtles->end())
    {
        float h = (*turtle_it)->get_bounding_box().y / 2;
        float w = (*turtle_it)->get_bounding_box().x / 2;
        if (std::abs((*turtle_it)->get_position().y) + h > screen.y + 400
                || std::abs((*turtle_it)->get_position().x) + w > screen.x + 400) {
            (*turtle_it)->destroy();
            turtle_it = m_turtles->erase(turtle_it);
            continue;
        } else {
            ++turtle_it;
        }
    }

    // for debugging purposes
    if (keyMap[GLFW_KEY_F]) {
        m_vamp_mode_charge = 15;
    }
    if (keyMap[GLFW_KEY_G]) {
        add_health(MAX_HEALTH);
    }

    if (keyMap[GLFW_KEY_ESCAPE]) {
        GameEngine::getInstance().changeState(new MainMenuState());
        return;
    }

    if (m_vamp_mode_charge >= 15 && keyMap[GLFW_KEY_ENTER]) {
        m_vamp_mode = true;
        m_vamp_mode_timer = VAMP_MODE_DURATION;
        m_vamp_mode_charge = 0;
        GameEngine::getInstance().setM_current_speed(0.5f);
        m_vamp_charge->setVampCharge(0);

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

    // Removing out of screen bullets
    // TODO move into player code? do same thing for boss/enemy bullets?
    bullet_it = playerBullets.begin();
    while(bullet_it != playerBullets.end()) {
        if ((*bullet_it)->isOffScreen(screen))
        {
            (*bullet_it)->destroy();
            bullet_it = playerBullets.erase(bullet_it);
            continue;
        }

        ++bullet_it;
    }


    // Boss specific code
    if (m_boss_mode) {
        // If boss drops below 0 health, set him as killed, award points, start timer
        if (m_boss->is_alive() && m_boss->getHealth() <= 0) {
            Mix_PlayMusic(m_victory_music, 0);
            m_boss->kill();
            m_points += 100;
            m_space.set_boss_dead();
        } else {
            // Player/Boss collision
            if (m_player->is_alive() && m_boss->collidesWith(*m_player) && m_player->get_iframes() <= 0.f) {
                m_player->set_iframes(500.f);
                lose_health(DAMAGE_ENEMY);
                Mix_PlayChannel(-1, m_player_explosion, 0);
                // TODO Knockback?
            }

            // Vamp/Boss collision
            if (m_vamp_mode && m_boss->collidesWith(m_vamp)) {
                // TODO sound effect, etc
                // TODO vamp mode adjustments, timer for this, etc
                m_boss->addDamage(VAMP_HEAL);
                add_health(VAMP_HEAL);
            }

            auto& bossBullets = m_boss->projectiles;

            // Checking Enemy Bullet - Player collisions
            auto boss_bullet_it = bossBullets.begin();
            while (boss_bullet_it != bossBullets.end()) {
                if ((*boss_bullet_it)->collides_with(*m_player))
                {
                    (*boss_bullet_it)->destroy();
                    boss_bullet_it = bossBullets.erase(boss_bullet_it);
                    if (m_player->is_alive() && m_player->get_iframes() <= 0.f) {
                        m_player->set_iframes(500.f);
                        lose_health(DAMAGE_BOSS);
                    }
                    break;
                } else {
                    ++boss_bullet_it;
                }
            }

            // Removing out of screen bullets
            // TODO move into boss class?
            boss_bullet_it = bossBullets.begin();
            while(boss_bullet_it != bossBullets.end()) {
                if ((*boss_bullet_it)->isOffScreen(screen))
                {
                    (*boss_bullet_it)->destroy();
                    boss_bullet_it = bossBullets.erase(boss_bullet_it);
                    continue;
                } else {
                    ++boss_bullet_it;
                }
            }

            m_boss->update(ms);

            // If boss dies, return to main menu
            if (m_boss->getHealth() <= 0 && m_space.get_boss_dead_time() > 5)
            {
                if (m_level.nextLevel != nullptr) {
                    GameEngine::getInstance().changeState(new LevelState(*m_level.nextLevel, m_points));
                } else {
                // TODO save m_points to a leaderboard?
                // TODO go to Epilogue state
                    GameEngine::getInstance().changeState(new MainMenuState());
                }
                return;
            }
        }
    }

    // If salmon is dead, restart the game after the fading animation
    if (!m_player->is_alive() &&
        m_space.get_salmon_dead_time() > 5) {
       reset();
    }
    //std::cout << "updateend" << std::endl;
}

void LevelState::draw() {
    // Clearing error buffer
    gl_flush_errors();

    // Getting size of window
    int w, h;
    GLFWwindow* m_window = GameEngine::getInstance().getM_window();
    glfwGetFramebufferSize(m_window, &w, &h);

    // Updating window title with points
    std::stringstream title_ss;
    title_ss << "Points: " << m_points;
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
    for (auto& turtle : (*m_turtles))
        turtle->draw(projection_2D);
    if (m_vamp_mode) {
        m_vamp.draw(projection_2D);
    }
    m_player->draw(projection_2D);
    if (m_boss_mode) {
        m_boss->draw(projection_2D);
    }
    m_uiPanel->draw(projection_2D);
    m_health->draw(projection_2D);
    m_vamp_charge->draw(projection_2D);
    m_dialogue.draw(projection_2D);
    m_explosion.draw(projection_2D);


    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);
}

void LevelState::lose_health(int damage) {
    m_player->lose_health(damage);
    Mix_PlayChannel(-1, m_player_dead_sound, 0);
    if (!m_player->is_alive()) {
        m_space.set_salmon_dead();
    }
}

void LevelState::add_health(int heal) {
    float dif = MAX_HEALTH -  m_player->get_health();

    int healVal = 0;
    if (dif >= heal)
        healVal = heal;
    else if (dif > 0)
        healVal = (int) dif;

    m_player->gain_health(healVal);
}

void LevelState::add_vamp_charge() {
    if (m_vamp_mode_charge < 15) {
        m_vamp_mode_charge++;
        
        if (m_vamp_mode_charge == 15) {
            // TODO - replace with more appropriate sound
            Mix_PlayChannel(-1, m_player_charged, 0);
        }
    }
}

void LevelState::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
    // Track which keys are being pressed or held
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[key] = true : keyMap[key] = false;

    // Resetting game
    if (action == GLFW_RELEASE && key == GLFW_KEY_R)
    {
        reset();
    }
}

void LevelState::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
    mouse_position.x = xpos;
    mouse_position.y = ypos;
}

void LevelState::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
    // Track which mouse buttons are being pressed or held
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[button] = true : keyMap[button] = false;
}

void LevelState::reset() {
    GameEngine::getInstance().changeState(new LevelState(m_level, 0));
}
