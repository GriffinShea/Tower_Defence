#pragma once
#include "GameObject.h"
class Explosion :
	public GameObject
{
public:
	Explosion(glm::vec3);
	void update(double);
	virtual void render(Renderer*);
private:
	float timer;
	float lifeTime;
};

