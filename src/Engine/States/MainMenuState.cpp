//
// Created by Cody on 9/17/2019.
//

#include <sstream>
#include "MainMenuState.hpp"
#include "LevelState.hpp"
#include "TutorialState.hpp"

Texture MainMenuState::bg_texture;

void MainMenuState::init() {
    if (!bg_texture.is_valid())
    {
        if (!bg_texture.load_from_file(textures_path("mainmenu_bg.png")))
        {
            throw std::runtime_error("Failed to load background texture!");
        }
    }
}

void MainMenuState::terminate() {

}

void MainMenuState::update(float ms) {

}

void MainMenuState::draw() {
    // Clearing error buffer
    gl_flush_errors();

    // Getting size of window
    int w, h;
    GLFWwindow* m_window = GameEngine::getInstance().getM_window();
    glfwGetFramebufferSize(m_window, &w, &h);

    // Updating window title with points
    std::stringstream title_ss;
    title_ss << "V.A.P.E";
    glfwSetWindowTitle(m_window, title_ss.str().c_str());

    /////////////////////////////////////
    // First render to the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, GameEngine::getInstance().getM_frame_buffer());

    // Clearing backbuffer
    glViewport(0, 0, w, h);
    glDepthRange(0.00001, 10);
    const float clear_color[3] = { 0.f, 0.f, 0.f };
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /////////////////////
    // Truely render to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clearing backbuffer
    glViewport(0, 0, w, h);
    glDepthRange(0, 10);
    glClearColor(0, 0, 0, 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO This is supposed to draw the background, but it seems a bit buggy at least on my computer - Cody
    GLuint fboId = 0;
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, bg_texture.id, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // if not already bound
    glBlitFramebuffer(0, 0, w, h, 0, h, w, 0,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    //

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GameEngine::getInstance().getM_screen_tex().id);


    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);

}

void MainMenuState::on_key(GLFWwindow *wwindow, int key, int i, int action, int mod) {
}

void MainMenuState::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
	mouse_position.x = xpos;
	mouse_position.y = ypos;
}

void MainMenuState::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
	if (mouse_position.x >= 200 && mouse_position.x <= 600 && mouse_position.y >= 600 && mouse_position.y <= 700) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			GameEngine::getInstance().changeState(new TutorialState());
		}
	}
}
