#include "PowerUp.h"

PowerUp::PowerUp(glm::vec3 powerupPosition, glm::vec3 powerupScale)
	: GameObject(powerupPosition, powerupScale)
{
	texture = "WizardPowerUp";
	timer = 0.0f;
	lifeTime = 5.0f;
}

void PowerUp::update(double deltaTime)
{
	timer += deltaTime;

	if (timer > lifeTime)
	{
		timer = 0;
		exist = false;
	}
}