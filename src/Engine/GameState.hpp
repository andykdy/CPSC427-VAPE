//! Abstract class representing a state of the game
/*!
 * This abstract class provides the base for GameStates,
 * the states used by the game engine state machine.
 *
 * @author: Cody Newman
 *
 * Created on: 9/16/2019
 */

#ifndef VAPE_GAMESTATE_HPP
#define VAPE_GAMESTATE_HPP

#include "./GameEngine.hpp"

class GameState {
public:
    //! Initializes the state
    virtual void init() = 0;

    //! Cleans up the state
    virtual void terminate() = 0;

    //! Updates the state
    virtual void update(float ms) = 0;

    //! Renders the state
    virtual void draw() = 0;

    //! Default destructor
    virtual ~GameState() = default;

    //! INPUT CALLBACK FUNCTIONS
    virtual void on_key(GLFWwindow*, int key, int, int action, int mod) = 0;
    virtual void on_mouse_move(GLFWwindow* window, double xpos, double ypos) = 0;
    virtual void on_mouse_button (GLFWwindow* window, int button, int action, int mods) = 0;
};



#endif //VAPE_GAMESTATE_HPP
