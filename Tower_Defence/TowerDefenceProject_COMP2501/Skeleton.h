#ifndef SKELETON_H
#define SKELETON_H

#include "Enemy.h"

const int SKELETONHEALTH = 15;
const int SKELETONLOOT = 10;
const float SKELETONSPEED = 0.25f;
const glm::vec3 SKELETONSCALE = glm::vec3(0.1, 0.2, 1);

class Skeleton : public Enemy {
	public:
		Skeleton(float, stack<Node*>, glm::vec3);

};
#endif

