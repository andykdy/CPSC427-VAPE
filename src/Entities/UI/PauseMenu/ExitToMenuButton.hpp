//
// Created by Cody on 11/19/2019.
//

#ifndef VAPE_SAVEEXITBUTTON_HPP
#define VAPE_SAVEEXITBUTTON_HPP


#include <Entities/UI/Button.hpp>

class ExitToMenuButton : public Button {
    static Texture exit_button_texture;
public:
    bool init(const vec2 &position, const vec2 &scale, float rotation) override;

    void destroy() override;

    void update(float ms, const vec2 &mouse_position) override;

    void draw(const mat3 &projection) override;

    bool isWithin(const vec2 &mouse_position) override;

    vec2 getPosition() override;

    void doAction() override;
};


#endif //VAPE_SAVEEXITBUTTON_HPP
