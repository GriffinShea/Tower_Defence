#include "Enemy.h"

Enemy::Enemy(float st, stack<Node*> p, int h, int l, float sp, glm::vec3 ps, glm::vec3 sc)
	: MobileObject(sp, ps, sc) {
	stagger = st;
	timer = 0.0f;
	health = h;
	loot = l;
	path = p;
	chance = 92;
	return;
}

void Enemy::update(double dTime, vector<PowerUp*> *ability, vector<Projectile*> *projectiles, int *currency, int *baseHealth)
{

	//check for collisions between all projectiles and this enemy even if its staggered/frozen
	for (int i = 0; i < projectiles->size(); i++) {
		if (glm::distance(position, projectiles->at(i)->getPosition()) < projectiles->at(i)->getScale().x) {//here, we treat the enemy as a circle collider
			//if there is a collision, this enemy takes damage and the projectile gets set to not exist, if this enemies health is less than 1 it is as well
			health -= projectiles->at(i)->getDamage();
			projectiles->at(i)->setExistance(false);
		}
	}

	//if this enemy has not waited longer than its stagger then wait some more, else...
	if (timer < stagger) {
		timer += dTime;
	} else {

		//if path is not empty, calculate velocity to next node on the path
		if (path.size() > 0) {
			glm::vec3 playerToPath = glm::vec3(path.top()->getPosition().x - position.x, path.top()->getPosition().y - position.y, 0);
			float angle = glm::atan(playerToPath.y, playerToPath.x);
			velocity = glm::vec3(std::cos(angle), std::sin(angle), 0) * 1.0f;
			if (glm::length(playerToPath) < 0.1f) {
				path.pop();
				if (path.size() == 0) { velocity = glm::vec3(0, 0, 0); }

			}
		}
		else {//when path is empty, delete self and decrease basehealth because this enemy is at the base
			exist = false;
			*baseHealth -= 1;
		}

		MobileObject::update(dTime);//move
	}

	if (health <= 0) {
		exist = false;
		*currency += loot;//add this enemy's loot to currency
		if (spawnPowerup())
		{
			ability->push_back(new PowerUp(glm::vec3(position), glm::vec3(0.1)));
		}
	}
}
// 8% cahnce to get a powerup
bool Enemy::spawnPowerup()
{
	float c = rand() % 100;

	if (chance < c)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy::damage(int d) {
	health -= d;
	cout << "    enemy is damaged" << endl;
}