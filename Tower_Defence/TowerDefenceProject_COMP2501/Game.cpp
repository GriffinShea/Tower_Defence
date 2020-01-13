#include "Game.h"

Game::Game(void)
{
	try
	{
		//RENDERING SETUP
		// Seed for generating random numbers with rand()
		srand((unsigned int)time(0));
		// Setup window
		window = new Window(window_width_g, window_height_g, window_title_g);
		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		// Enable Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);
		glDepthMask(GL_TRUE); // allow writes to depth buffer

		// Set up renderer
		renderer = new Renderer("spriteShader.vert", "fireParticleShader.vert", "explosionParticleShader.vert", "fragmentShader.frag", "textShader.vert", "textShader.frag");

		setupMenu();
	}
	catch (exception &e)
	{
		// print exception and sleep so error can be read
		PrintException(e);
		
		this_thread::sleep_for(chrono::milliseconds(100000));
	}
}

Game::~Game(void)
{
	delete window;
	delete renderer;
}

void Game::setupMenu(void)
{
	state = 0;
	currency = 0;
	waves = 0;
	powerup = 0;
	pickup = false;
	towers.clear();
	enemies.clear();
	projectiles.clear();
	wizards.clear();
	powerups.clear();
	explosions.clear();
	roundInfo = queue<array<int, 4>>();
	background = "Title_Screen";
}

void Game::setupLevel(string level) {
	ifstream levelFile("assets/level_" + level + ".txt");
	string line;

	//set background image (the maze)
	getline(levelFile, line);
	background = strdup(line.c_str());
	
	cout << background << "setup" << endl;

	//set currency
	getline(levelFile, line);
	currency = stoi(line);

	//set start and end node positions
	getline(levelFile, line);
	maze = new Maze(40, 30, "assets/" + line);
	//load rounds into round variable
	//in the file the rouns are stored in lines of x,y,z where x == num zombies, y == num skeletons, z == num necromancers, w == num waves
	//loop over the remaining lines and store in the info into round info
	while (getline(levelFile, line))
	{ 
		roundInfo.push({ stoi(line), stoi(line.substr(line.find(",") + 1)), stoi(line.substr(line.find(",", line.find(",") + 1) + 1)), stoi(line.substr(line.find(",", line.find(",", line.find(",") + 1) + 1) + 1))});
	}
	//reset base health
	baseHealth = 10;

}

void Game::input(double dTime) {
	//menu state
	if (state == 0) {
		//pressing 1, 2, or 3 will begin levels one to three
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_1) == GLFW_PRESS)
		{
			setupLevel("1");
			level = "1";
			state = 1;
		}
		else if (glfwGetKey(Window::getWindow(), GLFW_KEY_2) == GLFW_PRESS)
		{
			setupLevel("2");
			level = "2";
			state = 1;

		}else if (glfwGetKey(Window::getWindow(), GLFW_KEY_3) == GLFW_PRESS)
		{
			setupLevel("3");
			level = "3";
			state = 1;
		}
		//escape or closing the window will exit the game
		else if (glfwGetKey(Window::getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window->getWindow())) {state = 2;}
		
	}
	//game state
	else if (state == 1) 
	{
		//pressing q returns to the menu, quitting the window closes the game
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
			state = 0;
			setupMenu();
		}
		else if (glfwWindowShouldClose(window->getWindow())) { state = 2; }

		//if mouse is down and was up last tick, allow the player to place a tower
		if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseWasUp)
		{
			// get the cursor location on the screen
			double mouseX, mouseY;
			glfwGetCursorPos(Window::getWindow(), &mouseX, &mouseY);
			Node *position = maze->getNodeFromCoords(mouseX / (window_width_g / 2) - 1, 1 - mouseY / (window_height_g / 2));//get the node at that position
			
			if (!position->isOccupied())
			{
				// Placing a Lightning Tower and buying the tower
				if (glfwGetKey(Window::getWindow(), GLFW_KEY_L) == GLFW_PRESS && currency >= LIGHTNINGTOWERPRICE)
				{
					towers.push_back(new LightningTower(glm::vec3(mouseX / (window_width_g / 2) - 1, 1 - mouseY / (window_height_g / 2), 0)));
					currency -= LIGHTNINGTOWERPRICE;
					position->setTowerHere(true);
				}
				// Placing a Fire Tower and buying the tower
				else if (glfwGetKey(Window::getWindow(), GLFW_KEY_F) == GLFW_PRESS && currency >= FIRETOWERPRICE)
				{
					towers.push_back(new FireTower(glm::vec3(mouseX / (window_width_g / 2) - 1, 1 - mouseY / (window_height_g / 2), 0)));
					currency -= FIRETOWERPRICE;
					position->setTowerHere(true);
				}
				// Placing an Ice Tower and buying the tower
				else if (glfwGetKey(Window::getWindow(), GLFW_KEY_I) == GLFW_PRESS && currency >= ICETOWERPRICE)
				{
					towers.push_back(new IceTower(glm::vec3(mouseX / (window_width_g / 2) - 1, 1 - mouseY / (window_height_g / 2), 0)));
					currency -= ICETOWERPRICE;
					position->setTowerHere(true);
				}

			}
			// if the mouse is in range of the powerup 
			// erase the power up
			// increase the power up value
			// set the pickup to true
			for (int i = powerups.size() - 1; i >= 0; i--)
			{
				if ((mouseX / (window_width_g / 2) - 1) > powerups[i]->getPosition().x - powerups[i]->getScale().x /2 
					&& (mouseX / (window_width_g / 2) - 1) < powerups[i]->getPosition().x + powerups[i]->getScale().x / 2
					&& (1 - mouseY / (window_height_g / 2)) > powerups[i]->getPosition().y - powerups[i]->getScale().y / 2 
					&& (1 - mouseY / (window_height_g / 2)) < powerups[i]->getPosition().y + powerups[i]->getScale().y / 2)
				{
					powerup++;
					powerups.erase(powerups.begin() + i);
				}

			}
		}

		//wizard can be spawned by pressing w when and costs 3 powerup points, it is spawned with a path from one end on the maze to one start
		if (glfwGetKey(Window::getWindow(), GLFW_KEY_W) == GLFW_PRESS && keyWasUp_w && powerup >= 3)
		{
			powerup -= 3;
			stack<Node*> path = maze->getWizardPath();
			wizards.push_back(new Wizard(path.top()->getPosition(), path));
		}
		//set keyWasUp to true if key is up this tick
		keyWasUp_w = glfwGetKey(Window::getWindow(), GLFW_KEY_W) == GLFW_RELEASE;
		//set mouseWasUp to true if mouse is up this tick
		mouseWasUp = glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE;
	}
}

void Game::update(double dTime) {

	if (state == 1) {
		//if round is over (all enemies are dead) repopulate enemies based on roundInfo
		if (enemies.size() == 0) {
			if (roundInfo.size() == 0) { setupMenu(); }//if no more rounds, level is complete, go back to menu
			else {
				
				// get the enemy path for the creeps and start position
				stack<Node*> enemyPath = maze->getEnemyPath();
				glm::vec3 startPosition = enemyPath.top()->getPosition();
				
				//create zombies, skeletons, and necromancers and place them at the start node, with the enemy path
				//each enemy gets a stagger to make them wait a bit before moving out of the spawn
				float stagger = 0.0f;
				for (int i = 0; i < roundInfo.front()[0]; i++) {
					enemies.push_back(new Zombie(stagger, enemyPath, startPosition));
					stagger += 0.5f;
					cout << stagger;
				}
				for (int i = 0; i < roundInfo.front()[1]; i++) {
					enemies.push_back(new Skeleton(stagger, enemyPath, startPosition));
					stagger += 0.5f;
				}
				for (int i = 0; i < roundInfo.front()[2]; i++) {
					enemies.push_back(new Necromancer(stagger, enemyPath, startPosition));
					stagger += 0.5f;
				}
				// show the current round on screen
				waves = roundInfo.front()[3];

				roundInfo.pop();//pop round
			}
		}
		//update towers, projectiles, enemies and wizards
		for (int i = 0; i < explosions.size(); i++) { explosions[i]->update(dTime); }
		for (int i = 0; i < towers.size(); i++) { towers[i]->update(dTime, enemies, &projectiles); }
		for (int i = 0; i < projectiles.size(); i++) { projectiles[i]->update(dTime); }
		for (int i = 0; i < enemies.size(); i++) { enemies[i]->update(dTime, &powerups, &projectiles, &currency, &baseHealth); }
		for (int i = 0; i < wizards.size(); i++) { wizards[i]->update(dTime, enemies); }
		for (int i = 0; i < powerups.size(); i++) { powerups[i]->update(dTime); }

		if (powerup >= 4)
		{
			pickup = false;
		}

		//delete non existant towers
		for (int i = towers.size() - 1; i >= 0; i--) {
			if (!towers[i]->exists()) {
				towers.erase(towers.begin() + i);
			}
		}
		//delete non existant projectiles
		for (int i = projectiles.size() - 1; i >= 0; i--) {
			if (!projectiles[i]->exists()) {
				projectiles.erase(projectiles.begin() + i);
			}
		}
		//delete non existant enemies, add their loot to the currency
		for (int i = enemies.size() - 1; i >= 0; i--) {
			if (!enemies[i]->exists()) {
				//if enemy died, create an explosion
				if (enemies[i]->getHealth() <= 0) { explosions.push_back(new Explosion(enemies[i]->getPosition())); }
				enemies.erase(enemies.begin() + i);
			}
		}
		// delete non existant wizards
		for (int i = wizards.size() - 1; i >= 0; i--){
			if (!wizards[i]->exists()){
				//wizard explodes when it despawns
				explosions.push_back(new Explosion(wizards[i]->getPosition()));
				wizards.erase(wizards.begin() + i);
			}
		}
		// delete non existant powerups
		for (int i = powerups.size() - 1; i >= 0; i--) {
			if (!powerups[i]->exists()) {
				powerups.erase(powerups.begin() + i);
			}
		}
		// delete non existant explosions
		for (int i = explosions.size() - 1; i >= 0; i--) {
			if (!explosions[i]->exists()) {
				explosions.erase(explosions.begin() + i);
			}
		}
	//if no more base health game ends and returns to menu
		if (baseHealth <= 0) { setupMenu(); }
	}
}

void Game::render(void) {
	// Clear background
	window->clear(viewport_background_color_g);
	// Update other events like input handling
	glfwPollEvents();

	//draw game objects
	for (int i = 0; i < explosions.size(); i++) { explosions[i]->render(renderer); }
	for (int i = 0; i < towers.size(); i++) { towers[i]->render(renderer); }
	for (int i = 0; i < projectiles.size(); i++) { projectiles[i]->render(renderer); }
	for (int i = 0; i < enemies.size(); i++) { enemies[i]->render(renderer); }
	for (int i = 0; i < powerups.size(); i++) { powerups[i]->render(renderer); }
	for (int i = 0; i < wizards.size(); i++) { wizards[i]->render(renderer); }

	// render currency text
	if (state == 1)
	{
		if (level == "1")
		{
			renderer->renderText(glm::vec3(-0.68, 0.93, 0), 0.04f, string("Currency " + to_string(currency)));
			renderer->renderText(glm::vec3(0.4, 0.93, 0), 0.04f, string("Base health " + to_string(baseHealth)));
			renderer->renderText(glm::vec3(-0.15, 0.8, 0), 0.05f, string("Wave " + to_string(waves)));
			renderer->renderText(glm::vec3(-0.95, -0.95, 0), 0.06f, string("Pickups " + to_string(powerup)));
		}
		else if (level == "2")
		{
			renderer->renderText(glm::vec3(-0.95, 0.93, 0), 0.04f, string("Currency " + to_string(currency)));
			renderer->renderText(glm::vec3(0.12, 0.93, 0), 0.04f, string("Base health " + to_string(baseHealth)));
			renderer->renderText(glm::vec3(-0.95, -0.95, 0), 0.05f, string("Wave " + to_string(waves)));
			renderer->renderText(glm::vec3(0.18, -0.95, 0), 0.06f, string("Pickups " + to_string(powerup)));
		}
		else if (level == "3")
		{
			renderer->renderText(glm::vec3(-0.53, 0.93, 0), 0.04f, string("Currency " + to_string(currency)));
			renderer->renderText(glm::vec3(0.1, 0.93, 0), 0.04f, string("Base health " + to_string(baseHealth)));
			renderer->renderText(glm::vec3(-0.15, 0.8, 0), 0.05f, string("Wave " + to_string(waves)));
			renderer->renderText(glm::vec3(-0.95, -0.95, 0), 0.06f, string("Pickups " + to_string(powerup)));
		}
	}

	//draw background image with a scale transformation
	renderer->drawSprite(background, glm::scale(glm::mat4(1), glm::vec3(2, 2, 1)));

	// Push buffer drawn in the background onto the display
	glfwSwapBuffers(window->getWindow());
}
