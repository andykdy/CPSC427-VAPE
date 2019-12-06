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
#include <Systems/ProjectileSystem.hpp>
#include <Systems/PickupSystem.hpp>

#include "LevelState.hpp"
#include "MainMenuState.hpp"
#include "BetweenLevelsState.hpp"
#include "OutroState.hpp"

// Same as static in c, local to compilation unit
namespace
{
    const size_t VAMP_MODE_DURATION = 1500;
    const size_t MAX_HEALTH = 75;
    const size_t INIT_HEALTH = 50;
    const size_t DAMAGE_COLLIDE = 5;
    const size_t VAMP_HEAL = 2;
    const size_t VAMP_DAMAGE_TIMER = 125;
    const size_t VAMP_DAMAGE_TIMER_BOSS = 400;
    const size_t VAMP_DAMAGE_BOSS = 4;
    const size_t VAMP_TIME_PER_POINT = 200;
    const size_t VAMP_ACTIVATION_COOLDOWN = 300;
    const size_t MAX_VAMP_CHARGE = 15;
    const size_t VAMP_ACTIVATION_COST = 0;
    const float VAMP_TIME_SLOWDOWN = 0.5f;

}


LevelState::LevelState(Levels::Level level, PlayerData data) :
        m_level(level),
        m_points(data.points),
        m_starting_points(data.points),
        m_lives(data.lives),
        m_font_ranger(Font(font_path("spaceranger.ttf")))

{

    // Load leaderboard data
    auto leaderboard = getLeaderboard();

    m_highscore = 0;
    if (leaderboard.size() > 0)
        m_highscore = (--leaderboard.end())->points;
}

void LevelState::init() {
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

    // fprintf(stderr, "Loaded music\n");

    // Get screen size
    int w, h;
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

    m_vamp_mode = false;
    m_level_time = 0;
    m_boss_mode = false;
    m_boss_pre = false;
    m_vamp_cooldown = 0;
    m_vamp_mode_charge = 0;
    m_vamp_mode_timer = 0;
    m_numVampParticles = 0;
    m_debug_mode = false;
    m_player_invincibility = false;

    m_pause = &GameEngine::getInstance().getEntityManager()->addEntity<PauseMenu>();
    m_pause->init(screen);

    m_player = &GameEngine::getInstance().getEntityManager()->addEntity<Player>();
    m_player->init(screen, INIT_HEALTH);
    m_uiPanelBackground = &GameEngine::getInstance().getEntityManager()->addEntity<UIPanelBackground>();
    m_uiPanelBackground->init(screen, screen.y * 0.1f);
    m_health = &GameEngine::getInstance().getEntityManager()->addEntity<Health>();
    m_health->init({53, screen.y-50});
    m_vamp_charge = &GameEngine::getInstance().getEntityManager()->addEntity<VampCharge>();
    m_vamp_charge->init({screen.x-52, screen.y-50});
    m_vamp_particle_emitter.init();
    m_uiPanel = &GameEngine::getInstance().getEntityManager()->addEntity<UIPanel>();
    m_uiPanel->init(screen, screen.y, screen.x);
    m_dialogue.init(m_level.bossDialogue);
    m_dialogue.deactivate();
    m_space.init(m_level.backgroundTexture);
    m_explosion.init();
    m_boss = m_level.spawnBoss(GameEngine::getInstance().getEntityManager());
    m_boss->init(screen);

    m_space.set_position({screen.x/2, 0});
    // TODO - remove
    // m_font_ranger = Font(font_path("spaceranger.ttf"));

    GameEngine::getInstance().getSystemManager()->addSystem<MotionSystem>();
    auto & spawn = GameEngine::getInstance().getSystemManager()->addSystem<EnemySpawnerSystem>();
    spawn.reset(m_level.timeline);
    GameEngine::getInstance().getSystemManager()->addSystem<ProjectileSystem>();
	GameEngine::getInstance().getSystemManager()->addSystem<PickupSystem>();

    GameEngine::getInstance().setM_current_speed(1.f);



    PlayerData data{};
    data.points = m_starting_points;
    data.lives = m_lives;
    data.levelId = m_level.id;

    saveGameData(data);
}

void LevelState::terminate() {
    auto & spawn = GameEngine::getInstance().getSystemManager()->getSystem<EnemySpawnerSystem>();
    spawn.reset(m_level.timeline);

	auto& pickup_sys = GameEngine::getInstance().getSystemManager()->getSystem<PickupSystem>();
	pickup_sys.clear();

    auto & projectiles = GameEngine::getInstance().getSystemManager()->getSystem<ProjectileSystem>();
    projectiles.clear();

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

    m_pause->destroy();

    m_player->destroy();

    m_uiPanelBackground->destroy();
    m_uiPanel->destroy();
    m_health->destroy();
    m_vamp_charge->destroy();
    if (m_boss != nullptr)
        m_boss->destroy();
    m_dialogue.destroy();

    for (auto& text : m_text)
        text.destroy();

    m_explosion.destroy();
    m_space.destroy();

    m_vamp_particle_emitter.destroy();
}

void LevelState::update(float ms) {
    if (m_pause->isPaused()) {
        m_pause->update(ms, mouse_position, keyMap);
        return;
    }
    auto & projectiles = GameEngine::getInstance().getSystemManager()->getSystem<ProjectileSystem>();
    auto & spawn = GameEngine::getInstance().getSystemManager()->addSystem<EnemySpawnerSystem>();
	auto & pickup_sys = GameEngine::getInstance().getSystemManager()->getSystem<PickupSystem>();
	auto * pickups = pickup_sys.getPickups();
    auto * enemies = spawn.getEnemies();

    int w, h;
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };
    m_debug_mode = GameEngine::getInstance().getM_debug_mode();

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
        m_boss->set_position({screen.x/2, screen.y/10});
    }

    m_health->setHealth(m_player->get_health());
    m_vamp_charge->setVampCharge(m_vamp_mode_charge);

    // Update the player's position and screen size for the enemies
    for(auto& enemy: *enemies) {
        enemy->screen_size = screen;
        enemy->player_position = m_player->get_position();
    }

    // Checking Player - Enemy collisions
    auto enemy_it = enemies->begin();
    while (enemy_it != enemies->end())
    {
        if (m_player->collides_with(**enemy_it))
        {
            if (m_player->is_alive()) {
                if (m_player->get_iframes() <= 0.f) {
                    m_player->set_iframes(500.f);
                    lose_health(DAMAGE_COLLIDE);
                    Mix_PlayChannel(-1, m_player_explosion, 0);
                    (*enemy_it)->destroy();
                    enemy_it = enemies->erase(enemy_it);
                }
            }
            break;
        } else
        {
            enemy_it++;
        }
    }

    // Checking Enemy Bullet - Player collisions
    auto& bullets = projectiles.hostile_projectiles;

    // Checking Enemy Bullet - Player collisions
    auto bullet_it = bullets.begin();
    while (bullet_it != bullets.end()) {
        if ((*bullet_it)->collides_with(*m_player))
        {
            (*bullet_it)->destroy();
            if (m_player->is_alive() && m_player->get_iframes() <= 0.f) {
                m_player->set_iframes(500.f);
                lose_health((*bullet_it)->getDamage());
            }

            bullet_it = bullets.erase(bullet_it);
            break;
        } else {
            ++bullet_it;
        }
    }

    // Check Pickup Collisions
    auto pickup_it = pickups->begin();
    while (pickup_it != pickups->end()) {
        if ((*pickup_it)->collides_with(*m_player)) {
            (*pickup_it)->applyEffect(*m_player);
            (*pickup_it)->destroy();
            pickup_it = pickups->erase(pickup_it);
			break;
        }
		pickup_it++;
    }

    // Checking Player Bullet - Enemy collisions
    auto& playerBullets = projectiles.friendly_projectiles;
    bullet_it = playerBullets.begin();
    while (bullet_it != playerBullets.end())
    {
        bool eraseBullet = false;
        auto enemy_it = enemies->begin();
        while (enemy_it != enemies->end())
        {
            if ((*bullet_it)->collides_with(**enemy_it))
            {
                eraseBullet = true;

                m_text.emplace_back();
                m_text.back().init(&m_font_ranger);
                std::string s = std::to_string((*enemy_it)->get_points());
                char const *pchar = s.c_str();
                m_text.back().setText(pchar);
                m_text.back().setColor({1.f, 0.8f, 0.0f});
                m_text.back().setPosition((*enemy_it)->get_position());

                m_explosion.spawn((*enemy_it)->get_position());
                m_points += (*enemy_it)->get_points();
                (*enemy_it)->destroy();
                enemy_it = enemies->erase(enemy_it);
                Mix_PlayChannel(-1,m_player_explosion,0);
                add_vamp_charge();

                break;
            } else {
                ++enemy_it;
            }
        }
        if (m_boss_mode && m_boss->is_alive()) {
			if ((*bullet_it)->collides_with(*m_boss)) {
				eraseBullet = true;
				// TODO sound
				add_vamp_charge();
				m_boss->addDamage(2);
			} 
			if (m_boss->hasClones()) {
				auto& clones = m_boss->clones;
				auto clone_it = clones.begin();
				while (clone_it != clones.end()) {
					if ((*bullet_it)->collides_with(**clone_it)) {
						(*clone_it)->stun();
						eraseBullet = true;
					} 
					clone_it++;
				}
			}
        }
        if (eraseBullet) {
            (*bullet_it)->destroy();
            bullet_it = playerBullets.erase(bullet_it);
        } else
            ++bullet_it;
    }

    // add health if enough vampParticles
    m_numVampParticles += m_vamp_particle_emitter.getCapturedParticles();
    // 6 is average of particles dropped per kill
    if (m_numVampParticles >= 6) {
        add_health(VAMP_HEAL);
        m_numVampParticles = 0;
    }

    // check for vamp/enemy collisions
    m_vamp_cooldown -= ms;

    if (m_vamp_mode) {
        enemy_it = enemies->begin();
        while (enemy_it != enemies->end()) {
            if (m_vamp.collides_with(**enemy_it)) {
                // TODO - Re-add resetting vamp timer on leaving aura
                (*enemy_it)->add_vamp_timer(ms);

                if ((*enemy_it)->get_vamp_timer() >= VAMP_DAMAGE_TIMER) {
                    m_vamp_particle_emitter.spawn((*enemy_it)->get_position());
                    m_explosion.spawn((*enemy_it)->get_position());
                    m_points += (*enemy_it)->get_points();

                    m_text.emplace_back();
                    m_text.back().init(&m_font_ranger);
                    std::string s = std::to_string((*enemy_it)->get_points());
                    char const *pchar = s.c_str();
                    m_text.back().setText(pchar);
                    m_text.back().setColor({1.f, 0.8f, 0.0f});
                    m_text.back().setPosition((*enemy_it)->get_position());

                    Mix_PlayChannel(-1, m_player_explosion, 0);
                    (*enemy_it)->destroy();
                    enemy_it = enemies->erase(enemy_it);
                    continue;
                }

            }

            ++enemy_it;
        }
    }

    m_space.update(ms);

    auto text_it = m_text.begin();
    while (text_it != m_text.end()) {
        text_it->scroll_up(ms);
        if (!text_it->is_alive()) {
            (text_it)->destroy();
            text_it = m_text.erase(text_it);
            continue;
        }
        ++text_it;
    }


    m_vamp_particle_emitter.update(ms, m_player->get_position());
    m_explosion.update(ms);


    m_player->update(ms, keyMap, mouse_position);
	for (auto& enemy : *enemies)
		enemy->update(ms);
	/*for (auto& pkup : *pickups)
		pkup->update(ms);*/

    // Removing out of screen enemies
    enemy_it = enemies->begin();
    while (enemy_it != enemies->end())
    {
        float h = (*enemy_it)->get_bounding_box().y / 2;
        float w = (*enemy_it)->get_bounding_box().x / 2;
        if (std::abs((*enemy_it)->get_position().y) + h > screen.y + 400
            || std::abs((*enemy_it)->get_position().x) + w > screen.x + 400) {
            (*enemy_it)->destroy();
            enemy_it = enemies->erase(enemy_it);
            continue;
        } else {
            ++enemy_it;
        }
    }


    // debug state
    if (m_debug_mode)
    {

        if (keyMap[GLFW_KEY_F]) {
            add_health(MAX_HEALTH);
        }

        if (keyMap[GLFW_KEY_G]) {
            m_vamp_mode_charge = MAX_VAMP_CHARGE;
        }

        if (m_player_invincibility) {
            m_vamp_mode_charge = MAX_VAMP_CHARGE;
            add_health(MAX_HEALTH);
        }
    }

    bool end_vamp_mode = false;


    if (keyMap[GLFW_KEY_ENTER] && m_vamp_cooldown <= 0) {
        m_vamp_cooldown = VAMP_ACTIVATION_COOLDOWN;

        if (!m_vamp_mode && m_vamp_mode_charge >= VAMP_ACTIVATION_COST + 1)
        {
            m_vamp_mode = true;
            m_vamp_mode_charge -= VAMP_ACTIVATION_COST;

            GameEngine::getInstance().setM_current_speed(VAMP_TIME_SLOWDOWN);
            m_vamp_charge->setVampCharge(m_vamp_mode_charge);
            m_vamp.init(m_player->get_position());
        } else {
            end_vamp_mode = true;
        }
    }

    if (!m_player->is_alive()) {
        end_vamp_mode = true;
    }

    if (m_vamp_mode) {
        if (m_vamp_mode_charge <= 0 || end_vamp_mode) {
            GameEngine::getInstance().setM_current_speed(1.f);
            m_vamp_mode = false;
            m_vamp.destroy();
        } else {
            m_vamp.update(ms, m_player->get_position(), m_vamp_mode_charge);
            m_vamp_mode_timer += ms;
            if (m_vamp_mode_timer >= VAMP_TIME_PER_POINT) {
                m_vamp_mode_charge -= 1;
                m_vamp_mode_timer = 0;
            }
        }
    }

    // Boss specific code
    if (m_boss_mode) {
        m_boss->update(ms);
		m_boss->player_position = m_player->get_position();

        // If boss drops below 0 health, set him as killed, award points, start timer
        if (m_boss->is_alive() && m_boss->getHealth() <= 0) {
            Mix_PlayMusic(m_victory_music, 0);
            m_points += m_boss->get_points();
            // m_points += 5000;
            m_boss->kill();
            m_space.set_boss_dead();
        } else if (m_boss->is_alive()) {
            // Player/Boss collision
            if (m_player->is_alive() && m_boss->collidesWith(*m_player) && m_player->get_iframes() <= 0.f) {
                m_player->set_iframes(500.f);
                lose_health(DAMAGE_COLLIDE);
                Mix_PlayChannel(-1, m_player_explosion, 0);
                // TODO Knockback?
            }

            // Vamp/Boss collision
            if (m_vamp_mode && m_boss->collidesWith(m_vamp)) {
                m_boss->add_vamp_timer(ms);

                if (m_boss->get_vamp_timer() >= VAMP_DAMAGE_TIMER_BOSS) {
                    m_vamp_particle_emitter.spawn(m_boss->get_position());
                    m_vamp_particle_emitter.spawn(m_boss->get_position());
                    m_boss->addDamage(VAMP_DAMAGE_BOSS);
                    m_boss->reset_vamp_timer();
                }
            }

            auto& bossBullets = m_boss->projectiles;

            // Checking Enemy Bullet - Player collisions
            auto boss_bullet_it = bossBullets.begin();
            while (boss_bullet_it != bossBullets.end()) {
                if ((*boss_bullet_it)->collides_with(*m_player))
                {
                    if (m_player->is_alive() && m_player->get_iframes() <= 0.f) {
                        m_player->set_iframes(500.f);
                        lose_health((*boss_bullet_it)->getDamage());
                    }
                    if ((*boss_bullet_it)->shouldErase()){
                        (*boss_bullet_it)->destroy();
                        boss_bullet_it = bossBullets.erase(boss_bullet_it);
                    } else {
                        ++boss_bullet_it;
                    }
                    break;
                } else {
                    ++boss_bullet_it;
                }
            }

            // Removing out of screen bullets
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

            // If boss dies, continue to next level or main menu
        } else if (m_boss->getHealth() <= 0 && m_space.get_boss_dead_time() > 5)
        {
            if (m_level.nextLevel != nullptr) {
                GameEngine::getInstance().changeState(new BetweenLevelsState(m_level.nextLevel, m_starting_points, {
                        m_lives,
                        m_points,
                        m_level.nextLevel->id
                }));
            } else {
                saveScore(m_points);
                saveGameData({0,0,0}); // Clear savegame
                GameEngine::getInstance().changeState(new OutroState({
                        m_lives,
                        m_points,
                        0
                }));
            }
            return;
        }
    }

    // If salmon is dead, restart the game after the fading animation
    if (!m_player->is_alive() &&
        m_space.get_salmon_dead_time() > 5) {
        if (m_lives > 0){
            --m_lives;
            reset();
        }else {
            saveScore(m_points);
            GameEngine::getInstance().changeState(new MainMenuState()); // TODO game over state
        }
    }
}

void LevelState::draw() {
    auto & projectiles = GameEngine::getInstance().getSystemManager()->getSystem<ProjectileSystem>();
    auto & spawn = GameEngine::getInstance().getSystemManager()->addSystem<EnemySpawnerSystem>();
	auto & pickup_system = GameEngine::getInstance().getSystemManager()->getSystem<PickupSystem>();
	auto * pickups = pickup_system.getPickups();
    auto * enemies = spawn.getEnemies();
    
    // Clearing error buffer
    gl_flush_errors();

    // Getting size of window
    int w, h;
    GLFWwindow* m_window = GameEngine::getInstance().getM_window();
    glfwGetFramebufferSize(m_window, &w, &h);

    // Updating window title with points
    std::stringstream title_ss;
    title_ss << "Lives: " << m_lives << "          Points: " << m_points << "             High Score: " << ((m_points < m_highscore) ? m_highscore : m_points);
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
    for (auto* projectile : projectiles.hostile_projectiles)
        projectile->draw(projection_2D);
	for (auto& enemy : (*enemies))
		enemy->draw(projection_2D);
	for (auto& pickup : (*pickups))
		pickup->draw(projection_2D);
    for (auto* projectile : projectiles.friendly_projectiles)
        projectile->draw(projection_2D);
    m_player->draw(projection_2D);
    if (m_boss_mode) {
        m_boss->draw(projection_2D);
    }
    if (m_vamp_mode) {
        m_vamp.draw(projection_2D);
    }
    m_vamp_particle_emitter.draw(projection_2D);

    m_explosion.draw(projection_2D);

    m_uiPanelBackground->draw(projection_2D);
    m_health->draw(projection_2D);
    m_vamp_charge->draw(projection_2D);
    m_uiPanel->draw(projection_2D);
    m_dialogue.draw(projection_2D);

    for (auto& text : m_text) {
        text.draw(projection_2D);
    }




    m_pause->draw(projection_2D);

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
    if (!m_vamp_mode) {

        if (m_vamp_mode_charge < MAX_VAMP_CHARGE) {
            m_vamp_mode_charge++;

            if (m_vamp_mode_charge == MAX_VAMP_CHARGE) {
                Mix_PlayChannel(-1, m_player_charged, 0);
            }
        }
    }
}

void LevelState::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
    // Track which keys are being pressed or held
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? keyMap[key] = true : keyMap[key] = false;

    // Resetting game TODO should this be removed from final? Can cheese for lives
    if (action == GLFW_RELEASE && key == GLFW_KEY_R)
    {
        reset();
        return;
    }

    if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE)
    {
        m_pause->toggle();
        return;
    }

    if (action == GLFW_RELEASE && key == GLFW_KEY_V) {
        m_player_invincibility = !m_player_invincibility;
    }

    m_pause->on_key(wwindow, key, i, action, mod);
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
    GameEngine::getInstance().changeState(new LevelState(m_level, {m_lives, m_starting_points, m_level.id}));
}
