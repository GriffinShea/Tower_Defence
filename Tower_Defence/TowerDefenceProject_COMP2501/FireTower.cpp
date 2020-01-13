#include "FireTower.h"



FireTower::FireTower(glm::vec3 p) : Tower(FIRETOWERCOOLDOWN, FIRETOWERRANGE, FIRETOWERPRICE, p) {
	texture = "FireTower";
}


FireTower::~FireTower()
{
}

void FireTower::update(double dTime, vector<Enemy*> enemies, vector<Projectile*> *projectiles) {
	Tower::update(dTime, enemies, projectiles);
	//check if there is an enemy in range
	Enemy *enemy = findEnemyInRange(enemies);
	if (enemy != nullptr) {
		//if so, change firing to true and recalculate the direction of fire
		firing = true;
		direction = glm::vec2(glm::normalize(enemy->getPosition() - position));

		//if the tower's cooldown is ready iterate over all enemies and deal damage to the ones in range then reset the timer
		if (canShoot()) {
			float centerOfFlame = atan2(direction.x, direction.y);//calculate the angle of the center of the flame
			//damage each enemy, if it is in range, and its angle is within a third of pie of the center of the flame
			for (int i = 0; i < enemies.size(); i++) {
				float distance = glm::distance(position, enemies[i]->getPosition());
				if (distance < range) {
					if (fabs(centerOfFlame - atan2(glm::normalize(enemies[i]->getPosition() - position).x, glm::normalize(enemies[i]->getPosition() - position).y)) < PI / 3) {
						enemies[i]->damage(FIREDAMAGE);
					}
				}
			}
			resetTimer();
		}

	} else { firing = false; }
	
}

void FireTower::render(Renderer *renderer)
{
	//setup transform matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);//translate by position
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);//scale by scale
	glm::mat4 transformationMatrix = translationMatrix * scaleMatrix;//combine transformations

	//render game object
	renderer->drawSprite(texture, transformationMatrix);

	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	//render particles if firing (if an enemy is in range), direction is the direction of the flame
	if (firing) {renderer->drawFire("fire", translationMatrix * scaleMatrix, direction, 2);}
	
	
}
