#include "Tower.h"

Tower::Tower(float c, float r, int p, glm::vec3 po)
	:GameObject(po, TOWERSCALE) {//FIX SCALE LATER
	cooldown = c;
	range = r;
	price = p;
}

void Tower::update(double dTime, vector<Enemy*> enemies, vector<Projectile*> *projectiles) {
	timer += dTime;
}

Enemy * Tower::findEnemyInRange(vector<Enemy*> enemies)
{
	Enemy *target = nullptr;//this keeps the pointer to the closest enemy, it is the return value
	float targetDistance = numeric_limits<float>::max();//this keeps the distance to the closest enemy

	//iterate over enemies, calculate the distance between the enemy and this tower
	for (int i = 0; i < enemies.size(); i++) {
		float distance = glm::distance(position, enemies[i]->getPosition());
		//if distance less than range and the closest enemies distance, set target to this enemy and distance to this distance
		if (distance < range && distance < targetDistance) {
			targetDistance = distance;
			target = enemies[i];
		}
	}
	//return closest enemy
	return target;
}
