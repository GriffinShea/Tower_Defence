#ifndef GAME_H
#define GAME_H

// Macro for printing exceptions
#define PrintException(exception_object)\
	cerr << exception_object.what() << endl
#define GLEW_STATIC

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>

#include "Renderer.h"
#include "Window.h"
#include "MobileObject.h"
#include "Tower.h"
#include "Maze.h"
// enemies
#include "Zombie.h"
#include "Skeleton.h"
#include "Necromancer.h"
// towers
#include "FireTower.h"
#include "IceTower.h"
#include "LightningTower.h"
// powerUps
#include "PowerUp.h"
#include "Wizard.h"
//effects
#include "Explosion.h"

using namespace std;

class Game
{
public:
	Game(void);
	~Game(void);

	void input(double dTime);
	void update(double dTime);
	void render(void);
	
	inline int getState() { return state; };
private:
	//FUNCTIONS
	void setupMenu(void);
	void setupLevel(string);
	
	//CONSTANTS
	//constants for defining the OpenGL window and viewport
	const string window_title_g = "Tower Defence Demo";
	const unsigned int window_width_g = 800;
	const unsigned int window_height_g = 600;
	const glm::vec3 viewport_background_color_g = glm::vec3(0);

	//VARIABLES
	//holds whether the game is running
	int state; //0==menu, 1==game, 2==exit

	bool mouseWasUp;
	bool keyWasUp_w;
	bool pickup;

	int waves;
	int powerup;
	int currency;
	int baseHealth;

	vector<Enemy*> enemies;
	vector<Tower*> towers;
	vector<Projectile*> projectiles;
	vector<Wizard*> wizards;
	vector<PowerUp*> powerups;
	vector<Explosion*> explosions;
	
	//rounds are stored in a queue as arrays of three ints, representing number of zombies, skeletons, and necromancers
	queue<array<int, 4>> roundInfo;

	//maze used for pathfinding and stuff
	Maze *maze;
	
	//rendering variables
	Window *window;
	Renderer *renderer;
	
	//background variable
	string background;
	string level;

};

#endif // !GAME_H
