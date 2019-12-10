//
// Created by Cody on 11/19/2019.
//

#ifndef VAPE_RESUMEBUTTON_HPP
#define VAPE_RESUMEBUTTON_HPP

#include <Entities/UI/Button.hpp>

class ResumeButton : public Button {
    static Texture resume_button_texture;
public:
    bool init(const vec2 &position, const vec2 &scale, float rotation) override;

    void destroy() override;

    void update(float ms, const vec2 &mouse_position) override;

    void draw(const mat3 &projection) override;

    bool isWithin(const vec2 &mouse_position) override;

    vec2 getPosition() override;
};


#endif //VAPE_RESUMEBUTTON_HPP
