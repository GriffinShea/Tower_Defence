#ifndef MOBILEOBJECT_H
#define MOBILEOBJECT_H

#include "GameObject.h"

class MobileObject : public GameObject {
	public:
		MobileObject(float, glm::vec3, glm::vec3);
		//used to update position each tick
		virtual void update(double dTime);

	protected:
		glm::vec3 velocity;
		float speed;

};

#endif