//
// Created by Andrea Park on 2019-12-05.
//

#ifndef VAPE_WEAPONUI_HPP
#define VAPE_WEAPONUI_HPP

#include "common.hpp"
#include "Engine/ECS/ECS.hpp"

class WeaponUI: public ECS::Entity {
    static Texture ui_texture;

public:
    bool init(vec2 position);
    void update(float ms) override;
    void draw(const mat3& projection) override;
    void destroy() override;

    void setUI(const char *weapon_png);

private:

};

#endif //VAPE_WEAPONUI_HPP
