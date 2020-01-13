#include "Projectile.h"

Projectile::Projectile(glm::vec3 v, string t, int d, float s, glm::vec3 p)
	: MobileObject(s, p, PROJECTILESCALE)
{
	texture = t;
	damage = d;
	velocity = v;
}

void Projectile::update(double dTime)
{
	MobileObject::update(dTime);
	//if out of bounds, set exist to false
	if (position.x > 1 || position.x < -1 || position.y > 1 || position.y < -1) { exist = false; }
}

