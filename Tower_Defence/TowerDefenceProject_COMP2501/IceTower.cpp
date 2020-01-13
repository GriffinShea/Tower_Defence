#include "IceTower.h"



IceTower::IceTower(glm::vec3 p) : Tower(ICETOWERCOOLDOWN, ICETOWERRANGE, ICETOWERPRICE, p) {
	texture = "IceTower";
}


IceTower::~IceTower()
{
}

void IceTower::update(double dTime, vector<Enemy*> enemies, vector<Projectile*> *projectiles) {
	//parent class update
	Tower::update(dTime, enemies, projectiles);

	//if can shoot find the closest enemy if there are any in range and create a projectile that moves towards it
	if (canShoot()) {
		Enemy *enemy = findEnemyInRange(enemies);
		if (enemy != nullptr) {
			resetTimer();
			projectiles->push_back(new Projectile(glm::normalize(enemy->getPosition() - position), ICEPROJECTILETEXTURE, ICEPROJECTILEDAMAGE, ICEPROJECTILESPEED, position));
		}
	}
}
