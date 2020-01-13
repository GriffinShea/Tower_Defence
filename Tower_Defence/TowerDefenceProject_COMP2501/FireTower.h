#ifndef FIRETOWER_H
#define FIRETOWER_H

#include "Tower.h"

const float FIRETOWERRANGE = 0.375f;
const float FIRETOWERCOOLDOWN = 0.75f;
const int FIRETOWERPRICE = 50;
const int FIREDAMAGE = 2;

class FireTower :
	public Tower
{
public:
	FireTower(glm::vec3);
	~FireTower();

private:
	virtual void update(double, vector<Enemy*>, vector<Projectile*>*);
	virtual void render(Renderer*);

	bool firing;
	glm::vec2 direction;
};

#endif // !FIRETOWER_H
