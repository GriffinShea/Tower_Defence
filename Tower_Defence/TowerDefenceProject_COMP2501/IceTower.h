#ifndef ICETOWER_H
#define ICETOWER_H

#include "Tower.h"

const float ICETOWERCOOLDOWN = 1.0f;
const float ICETOWERRANGE = 0.5f;
const float ICETOWERPRICE = 25;

const string ICEPROJECTILETEXTURE = "rock";
const int ICEPROJECTILEDAMAGE = 5;
const float ICEPROJECTILESPEED = 1.0f;

class IceTower :
	public Tower
{
public:
	virtual void update(double, vector<Enemy*>, vector<Projectile*>*);
	IceTower(glm::vec3);
	~IceTower();
};
#endif // !ICETOWER_H

