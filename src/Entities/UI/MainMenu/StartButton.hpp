//
// Created by Cody on 11/14/2019.
//

#ifndef VAPE_STARTBUTTON_HPP
#define VAPE_STARTBUTTON_HPP


#include <Entities/UI/Button.hpp>

class StartButton : public Button {
    static Texture start_button_texture;
public:
    bool init(const vec2 &position, const vec2 &scale, float rotation) override;

    void destroy() override;

    void update(float ms, const vec2 &mouse_position) override;

    void draw(const mat3 &projection) override;

    bool isWithin(const vec2 &mouse_position) override;

    void doAction() override;

    vec2 getPosition() override;
};


#endif //VAPE_STARTBUTTON_HPP
