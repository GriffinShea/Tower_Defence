#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Enemy.h"

const int ZOMBIEHEALTH = 10;
const int ZOMBIELOOT = 5;
const float ZOMBIESPEED = 0.2f;
const glm::vec3 ZOMBIESCALE = glm::vec3(0.1, 0.2, 1);

class Zombie : public Enemy {
	public:
		Zombie(float, stack<Node*>, glm::vec3);

};
#endif