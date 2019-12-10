//
// Created by Cody on 11/14/2019.
//

#ifndef VAPE_TUTORIALBUTTON_HPP
#define VAPE_TUTORIALBUTTON_HPP

#include <Entities/UI/Button.hpp>

class TutorialButton : public Button {
    static Texture tutorial_button_texture;
public:
    bool init(const vec2 &position, const vec2 &scale, float rotation) override;

    void destroy() override;

    void update(float ms, const vec2 &mouse_position) override;

    void draw(const mat3 &projection) override;

    bool isWithin(const vec2 &mouse_position) override;

    void doAction() override;

    vec2 getPosition() override;
};


#endif //VAPE_TUTORIALBUTTON_HPP
