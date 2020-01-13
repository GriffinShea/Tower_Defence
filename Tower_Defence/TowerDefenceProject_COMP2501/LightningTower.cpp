#include "LightningTower.h"

#include <glm/gtx/rotate_vector.hpp>


LightningTower::LightningTower(glm::vec3 p) : Tower(LIGHTNINGTOWERCOOLDOWN, LIGHTNINGTOWERRANGE, LIGHTNINGTOWERPRICE, p) {
	texture = "LightningTower";
	firing = false;
}


LightningTower::~LightningTower()
{
}

void LightningTower::update(double dTime, vector<Enemy*> enemies, vector<Projectile*> *projectiles) {
	Tower::update(dTime, enemies, projectiles);
	//check if there is an enemy in range and the tower can shoot
	Enemy *enemy = findEnemyInRange(enemies);
	if (enemy != nullptr && canShoot()) {
		//if so, calculate collision and set firing to true, then iterate over enemies to check for collisions
		firing = true;
		direction = glm::vec2(glm::normalize(position - enemy->getPosition()));

		glm::vec3 lightningCollisionLine = position + glm::rotate(glm::vec3(range, 0, 0), (float)(atan2(direction.y, direction.x) * 180 / PI) - 180, glm::vec3(0, 0, 1));;
		glm::vec3 collisionNormal = glm::normalize(lightningCollisionLine);
		glm::vec3 centerOfLine = lightningCollisionLine / 2.0f;

		//damage each enemy, if it is close enough to the lightningCollisionLine
		for (int i = 0; i < enemies.size(); i++) {
			glm::vec3 centerOfLineToEnemy = enemies[i]->getPosition() - centerOfLine;
			glm::vec3 parallelComponent = glm::dot(centerOfLineToEnemy, collisionNormal) * collisionNormal;
			glm::vec3 perpendicularComponent = centerOfLineToEnemy - parallelComponent;
			float distanceTo = glm::distance(position, enemies[i]->getPosition());
			if (glm::length(perpendicularComponent) < range/4 && glm::length(lightningCollisionLine) / 2 > glm::length(parallelComponent)) {//here, we treat the enemy as a circle
				enemies[i]->damage(LIGHTNINGDAMAGE);
			}
		}
		resetTimer();
		

	}
	else if (timer > LIGHTNINGTOWERCOOLDOWN / 10) { firing = false; }
}

void LightningTower::render(Renderer *renderer)
{
	//setup transform matrix for tower
	glm::mat4 translationMatrix = glm::translate(position);//translate by position
	glm::mat4 scaleMatrix = glm::scale(scale);//scale by scale
	glm::mat4 towerTransformationMatrix = translationMatrix * scaleMatrix;//combine transformations
	//render tower
	renderer->drawSprite(texture, towerTransformationMatrix);

	//render lightning if firing
	if (firing) {
		//these are used by both the lightning texture and the explosion
		//this adjusts the scale back to 1,1 from the towers scale, so rotation works properly
		glm::mat4 adjustScale = glm::scale(glm::vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
		//rotate according to direction
		glm::mat4 orbitRotation = glm::rotate((float)(atan2(direction.y, direction.x) * 180 / PI) - 180, glm::vec3(0, 0, 1));//rotate according to direction
		
		//setup transformations for lightning and then draw
		glm::mat4 orbitTranslation = glm::translate(glm::vec3(range/2, 0, 0));//rotate half way out so the sprite is centered
		glm::mat4 rotateInPlace = glm::rotate(-90.0f, glm::vec3(0, 0, 1));//adjust it so its not the wrong way around
		scaleMatrix = glm::scale(glm::vec3(range/4, range, 1));//scale based on range
		glm::mat4 lightningTransformationMatrix = towerTransformationMatrix * adjustScale * orbitRotation * orbitTranslation * rotateInPlace * scaleMatrix;
		renderer->drawSprite("lightning1", lightningTransformationMatrix);

		//setup transformations for explosion and then draw
		orbitTranslation = glm::translate(glm::vec3(range, 0, 0));//translate all the way out
		scaleMatrix = glm::scale(glm::vec3(0.1));//scale down
		glm::mat4 explosionTransformationMatrix = towerTransformationMatrix * adjustScale * orbitRotation * orbitTranslation * scaleMatrix;
		renderer->drawExplosion("lightning2", explosionTransformationMatrix, timer * LIGHTNINGTOWERCOOLDOWN * 10);
	}


}
