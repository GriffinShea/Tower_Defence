#include "MobileObject.h"


MobileObject::MobileObject(float sp, glm::vec3 p, glm::vec3 sc)
	:GameObject(p, sc) {
	velocity = glm::vec3(0);
	speed = sp;
	return;
}

void MobileObject::update(double dTime) {
	//update position
	position += velocity * speed * (float)dTime;
	return;
}