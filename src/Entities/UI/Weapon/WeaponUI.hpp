//
// Created by matte on 2019-12-06.
//

#ifndef VAPE_WEAPONUI_HPP
#define VAPE_WEAPONUI_HPP

#include <Engine/ECS/ECS.hpp>
#include <Entities/UI/Text.hpp>

class WeaponUI : public ECS::Entity {

public:
    bool init(Font* font_ranger, Font* font_condensed);
    void update(float ms) override;
    void draw(const mat3& projection) override;
    void destroy() override;
    void setAmmo(float ammo);

private:
    bool alt_wep;
    Text inf_text;
    Text ammo_text;
    Font* ammo_font;
    Font* inf_font;
};


#endif //VAPE_WEAPONUI_HPP
