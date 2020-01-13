#ifndef ENEMY_H
#define ENEMY_H

#include "MobileObject.h"
#include <stack>
#include "Node.h"
#include "Projectile.h"
#include "PowerUp.h"

class Enemy : public MobileObject {
	public:
		Enemy(float, stack<Node*>, int, int, float, glm::vec3, glm::vec3);
		// Update function for moving the object around by its path
		virtual void update(double, vector<PowerUp*>*,vector<Projectile*>*, int*, int*);
		void damage(int d);
		bool spawnPowerup();

		inline void addStagger(float s) { stagger += s; };
		inline int getHealth() { return health; }
	protected:
		int health;
		int loot;
		float chance;
		float stagger;
		float timer;
		stack<Node*> path;

};

#endif