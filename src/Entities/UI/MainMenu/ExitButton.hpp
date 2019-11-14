//
// Created by Cody on 11/14/2019.
//

#ifndef VAPE_EXITBUTTON_HPP
#define VAPE_EXITBUTTON_HPP


#include <Entities/UI/Button.hpp>

class ExitButton : public Button {
public:
    bool init(const vec2 &position, const vec2 &scale, float rotation) override;

    void update(float ms, const vec2 &mouse_position) override;

    void draw(const mat3 &projection) override;

    bool isClicked(const vec2 &mouse_position) override;

    void doAction() override;
};


#endif //VAPE_EXITBUTTON_HPP
