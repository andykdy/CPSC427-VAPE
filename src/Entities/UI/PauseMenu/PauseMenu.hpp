//
// Created by Cody on 11/19/2019.
//

#ifndef VAPE_PAUSEMENU_HPP
#define VAPE_PAUSEMENU_HPP

#include <Engine/ECS/Entity.hpp>
#include <map>
#include <Entities/UI/Cursor.hpp>
#include "ResumeButton.hpp"
#include "ControlDiagram.hpp"
#include "ExitToMenuButton.hpp"

class PauseMenu : public ECS::Entity {
    static Texture pause_menu_texture;
private:
    bool m_paused = false;
    float m_resume_speed = 1.f;

    vec2 m_position;

    ResumeButton* m_resume;
    ExitToMenuButton* m_exit;
    ControlDiagram* m_controls;
    Cursor* m_cursor;
public:
    bool init(const vec2& screen);
    void destroy() override;
    void update(float ms, const vec2& mouse_position, std::map<int, bool> &keyMap);
    void draw(const mat3 &projection) override;
    void toggle();
    inline bool isPaused() const { return m_paused; };
    void on_key(GLFWwindow *wwindow, int key, int i, int action, int mod);
};


#endif //VAPE_PAUSEMENU_HPP
