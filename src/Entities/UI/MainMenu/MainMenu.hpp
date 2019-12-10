//
// Created by Cody on 10/21/2019.
//

#ifndef VAPE_MAINMENU_HPP
#define VAPE_MAINMENU_HPP


#include <common.hpp>

class MainMenu : public EntityOld {
    static Texture bg_texture;
public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all associated resources
    void destroy();

    // Renders the water
    void draw(const mat3& projection)override;
};


#endif //VAPE_MAINMENU_HPP
