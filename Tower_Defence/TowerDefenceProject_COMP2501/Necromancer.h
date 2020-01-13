#ifndef NECROMANCER_H
#define NECROMANCER_H

#include "Enemy.h"

const int NECROMANCERHEALTH = 10;
const int NECROMANCERLOOT = 25;
const float NECROMANCERSPEED = 0.4f;
const glm::vec3 NECROMANCERSCALE = glm::vec3(0.1, 0.2, 1);

class Necromancer : public Enemy {
	public:
		Necromancer(float, stack<Node*>, glm::vec3);

};
#endif
