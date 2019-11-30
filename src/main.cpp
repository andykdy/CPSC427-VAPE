/*!
 * @author: Cody Newman
 *
 * Created on: 9/16/2019
 */

#include <chrono>
#include <iostream>
#include <physfs.hpp>
#include "Engine/GameEngine.hpp"
#include "Engine/States/MainMenuState.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

using Clock = std::chrono::high_resolution_clock;

int main(int argv, char** args) {
	PHYSFS_init(args[0]);
	if (!PHYSFS_mount("../assets.vapepak", "", 1)){
        std::cout << "Unable to mount assets.vapepak, ensure it exists" << std::endl;
		std::cout << "Press any key to exit" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}
	GameEngine& game = GameEngine::getInstance();
	try {
		game.init();
	}
	catch(std::runtime_error &e){
		printf(e.what());
        // Time to read the error message
        std::cout << std::endl << "Press any key to exit" << std::endl;
        std::cin.get();
		return EXIT_FAILURE;
	}
	GameState *start = new MainMenuState;
	game.changeState(start);

    auto t = Clock::now();

	while(game.isRunning()){
        glfwPollEvents();

        // Calculating elapsed times in milliseconds from the previous iteration
        auto now = Clock::now();
        float elapsed_sec = (float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
        t = now;

		game.update(elapsed_sec);
		game.draw();
	}

	game.terminate();

	return EXIT_SUCCESS;
}