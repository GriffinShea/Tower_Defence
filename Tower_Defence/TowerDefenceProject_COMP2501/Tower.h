#ifndef TOWER_H
#define TOWER_H

#include "GameObject.h"
#include "Enemy.h"
#include "Projectile.h"

const glm::vec3 TOWERSCALE = glm::vec3(0.1, 0.2, 1);

class Tower : public GameObject {
	public:
		Tower(float, float, int, glm::vec3);
		//adds time to counter
		virtual void update(double, vector<Enemy*>, vector<Projectile*>*);
		//iterates over enemies finds out which one is closest and returns if in range
		Enemy *findEnemyInRange(vector<Enemy*>);
		
	protected:
		float timer = 0.0f;
		float cooldown;
		float range;
		int price;

		inline void resetTimer() { timer = 0; };
		inline bool canShoot() { return timer > cooldown; }

};

#endif