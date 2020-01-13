#ifndef WIZARD_H
#define WIZARD_H
#include "MobileObject.h"
#include "Enemy.h"

const float WIZARDSPEED = 0.2f;
const glm::vec3 WIZARDSCALE = glm::vec3(0.1, 0.2, 1);

const float WIZARDRANGE = 0.375f;

class Wizard : public MobileObject {
	public:
		Wizard(glm::vec3, stack<Node*>);
		//updates timer and calls parent update
		void update(double dTime, vector<Enemy*> enemies);
		Enemy *Wizard::findEnemyInRange(vector<Enemy*> enemies);
		virtual void render(Renderer*);

	private:
		float timer;
		float lifeTime;
		bool firing;
		glm::vec2 direction;
		stack<Node*> path;
};
#endif //!WIZARD_H

