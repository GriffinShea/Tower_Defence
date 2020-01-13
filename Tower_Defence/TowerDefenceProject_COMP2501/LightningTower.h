#ifndef LIGHTNINGTOWER_H
#define LIGHTNINGTOWER_H

#include "Tower.h"

const float LIGHTNINGTOWERCOOLDOWN = 1.5f;
const float LIGHTNINGTOWERRANGE = 0.5f;
const int LIGHTNINGTOWERPRICE = 75;
const int LIGHTNINGDAMAGE = 7;


class LightningTower :
	public Tower
{
public:
	LightningTower(glm::vec3);
	~LightningTower();

private:
	virtual void update(double, vector<Enemy*>, vector<Projectile*>*);
	virtual void render(Renderer*);

	bool firing;
	glm::vec2 direction;
};

#endif // !LIGHTNINGTOWER_H
