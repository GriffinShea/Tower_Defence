/*
 *	Authors: - Griffin Shea: griffin.shea@carleton.ca
 *			 - Houssam Rahal: houssam.rahal@carleton.ca
*/

#include "Game.h"

int main(void) {
	Game game = Game();
	double lastTime = glfwGetTime();
	double dTime;
	double currentTime;

	while (game.getState() != 2) {
		currentTime = glfwGetTime();
		dTime = currentTime - lastTime;
		lastTime = currentTime;
		game.input(dTime);
		game.update(dTime);
		game.render();
	}
}
