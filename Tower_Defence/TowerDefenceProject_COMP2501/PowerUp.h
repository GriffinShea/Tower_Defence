#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"

class PowerUp : public GameObject
{
public:
	PowerUp(glm::vec3, glm::vec3);
	void update(double);
private:
	float timer;
	float lifeTime;
};
#endif

