//
// Created by matte on 2019-12-06.
//

#ifndef VAPE_LIVES_HPP
#define VAPE_LIVES_HPP

#include <Engine/ECS/ECS.hpp>
#include <Entities/UI/Text.hpp>


class Lives : public ECS::Entity{

    static Texture lives_point_texture;

public:
    bool init(vec2 position, Font* font, int player_lives);
    void update(float ms) override;
    void draw(const mat3& projection) override;
    void destroy() override;

    void setLives(int lives);

private:
    int lives = 1;
    Text lives_text;
    Font* lives_font;
};


#endif //VAPE_LIVES_HPP
