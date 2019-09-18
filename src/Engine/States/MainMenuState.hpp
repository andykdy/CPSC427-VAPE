//! Game state representing the main menu
/*!
 * The MainMenu represents the game state
 * for the opening splash screen
 *
 * @author: Cody Newman
 *
 * Created on: 9/17/2019
 */

#ifndef VAPE_MAINMENUSTATE_HPP
#define VAPE_MAINMENUSTATE_HPP


#include <Engine/GameState.hpp>

class MainMenuState : public GameState{
    static Texture bg_texture;
public:
    void init(GameEngine *game) override;

    void terminate() override;

    void update(GameEngine *game) override;

    void draw(GameEngine *game) override;

    void on_key(GameEngine *game, GLFWwindow *wwindow, int key, int i, int action, int mod) override;

    void on_mouse_move(GameEngine *game, GLFWwindow *window, double xpos, double ypos) override;

    void on_mouse_button(GameEngine *game, GLFWwindow *window, int button, int action, int mods) override;

private:

};


#endif //VAPE_MAINMENUSTATE_HPP
