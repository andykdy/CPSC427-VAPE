//
// Created by Cody on 11/19/2019.
//

#include <Engine/GameEngine.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "PauseMenu.hpp"

Texture PauseMenu::pause_menu_texture;

bool PauseMenu::init(const vec2 &screen) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!pause_menu_texture.is_valid())
    {
        if (!pause_menu_texture.load_from_file(textures_path("pausemenu.png")))
        {
            throw std::runtime_error("Failed to load exit button texture");
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    if (!sprite->initTexture(&pause_menu_texture))
        throw std::runtime_error("Failed to initialize exit button sprite");

    m_position = {screen.x / 2, screen.y / 2};

    // Setup buttons/cursor

    vec2 buttonpos = {screen.x *0.5f, screen.y *0.65f};
    vec2 buttonscale = {0.75f, 0.75f};
    float buttonHeight = 150.f; // TODO
    float offset = 20.f;


    m_cursor = &GameEngine::getInstance().getEntityManager()->addEntity<Cursor>();
    m_cursor->init({buttonpos.x - 210, buttonpos.y}, buttonscale, 0);

    m_resume = &GameEngine::getInstance().getEntityManager()->addEntity<ResumeButton>();
    m_resume->init(buttonpos, buttonscale, 0);
    m_resume->select();

    buttonpos.y += buttonHeight + offset;

    m_exit = &GameEngine::getInstance().getEntityManager()->addEntity<ExitToMenuButton>();
    m_exit->init(buttonpos, buttonscale, 0);

    buttonpos.y += buttonHeight + offset;

    m_controls = &GameEngine::getInstance().getEntityManager()->addEntity<ControlDiagram>();
    m_controls->init({screen.x/2, screen.y*0.33f}, {0.7f,0.7f}, 0);

    return true;
}

void PauseMenu::destroy() {
    m_cursor->destroy();
    m_resume->destroy();
    m_exit->destroy();
    m_controls->destroy();

    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();

    ECS::Entity::destroy();
}

void PauseMenu::update(float ms, const vec2 &mouse_position, std::map<int, bool> &keyMap) {
    if (!m_paused) {
        return;
    }
    if (keyMap[GLFW_MOUSE_BUTTON_LEFT] && m_resume->isWithin(mouse_position)) {
        toggle();
        return;
    } else if (keyMap[GLFW_MOUSE_BUTTON_LEFT] && m_exit->isWithin(mouse_position)) {
        m_exit->doAction();
        return;
    }
}

void PauseMenu::draw(const mat3 &projection) {
    if (!m_paused) {
        return;
    }
    if (m_resume->isSelected()) {
        const vec2& pos = m_resume->getPosition();
        m_cursor->setPosition({pos.x-210, pos.y});
    } else {
        const vec2& pos = m_exit->getPosition();
        m_cursor->setPosition({pos.x-210, pos.y});
    }

    // draw menu bg
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    transform->begin();
    transform->translate(m_position);
    transform->scale({1,1});
    transform->rotate(0);
    transform->end();

    sprite->draw(projection, transform->out, effect->program);

    m_resume->draw(projection);
    m_exit->draw(projection);
    m_cursor->draw(projection);
    m_controls->draw(projection);
}

void PauseMenu::toggle() {
    if (GameEngine::getInstance().getM_current_speed() == 0.f) {
        m_paused = false;
        GameEngine::getInstance().setM_current_speed(m_resume_speed);
        Mix_ResumeMusic();
    } else {
        m_paused = true;
        m_resume_speed = GameEngine::getInstance().getM_current_speed(); // In case in vamp mode
        GameEngine::getInstance().setM_current_speed(0.f);
        Mix_PauseMusic();
    }
}

void PauseMenu::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
    if (!m_paused) {
        return;
    }
    if (action == GLFW_RELEASE && (key == GLFW_KEY_UP || key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_DOWN)) {
        if (m_resume->isSelected()) {
            m_resume->delesect();
            m_exit->select();
        } else {
            m_exit->delesect();
            m_resume->select();
        }
    }
    if (action == GLFW_RELEASE && key == GLFW_KEY_ENTER) {
        if (m_resume->isSelected()) {
            toggle();
            return;
        } else {
            m_exit->doAction();
            return;
        }
    }
}
