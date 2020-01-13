#include "Explosion.h"


Explosion::Explosion(glm::vec3 p)
	: GameObject(p, glm::vec3(0.1))
{
	texture = "fire";
	timer = 0.0f;
	lifeTime = 1.0f;
}

void Explosion::update(double deltaTime)
{
	//add to timer until lifetime is over
	timer += deltaTime;

	if (timer > lifeTime)
	{
		timer = 0;
		exist = false;
	}
}

void Explosion::render(Renderer *renderer) {
	//draw an explosion
	renderer->drawExplosion(texture, glm::scale(glm::translate(glm::mat4(1.0f), position), scale), timer);
}