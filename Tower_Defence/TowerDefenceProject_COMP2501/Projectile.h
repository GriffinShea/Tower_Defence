#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "MobileObject.h"

const glm::vec3 PROJECTILESCALE = glm::vec3(0.05, 0.05, 1);

class Projectile :
	public MobileObject {

	public:
		Projectile(glm::vec3, string, int, float, glm::vec3);
		virtual void update(double dTime);

		inline int getDamage() { return damage; };

	private:
		int damage;
};

#endif //!PROJECTILE_H