#include "Wizard.h"

//NOTE: textures could be stored somewhere outside the game object and be pulled directly from that place
//scale will be dependant on texture
Wizard::Wizard(glm::vec3 p, stack<Node*> pa)
	: MobileObject(WIZARDSPEED, p, WIZARDSCALE) {
	texture = "Wizard";
	timer = 0.0f;
	lifeTime = 2.0f;
	path = pa;
	firing = false;
	return;
}

void Wizard::update(double dTime, vector<Enemy*> enemies) {
	//if an enemy is in range change firing to true, stop, for 2 seconds the wizard will freeze enemies by adding to their stagger
	Enemy *enemy = findEnemyInRange(enemies);
	if (enemy != nullptr) {
		timer += dTime;
		velocity = glm::vec3(0, 0, 0);
		firing = true;

		//this algorith comes from the fire tower only it freezes enemies instead of damaging them
		//iterate over all enemies and stagger the ones in range 
		direction = glm::vec2(glm::normalize(enemy->getPosition() - position));
		float centerOfFlame = atan2(direction.x, direction.y);//calculate the angle of the center of the flame
		//damage each enemy, if it is in range, and its angle is within a third of pie of the center of the flame
		for (int i = 0; i < enemies.size(); i++) {
			float distance = glm::distance(position, enemies[i]->getPosition());
			if (distance < WIZARDRANGE) {
				if (fabs(centerOfFlame - atan2(glm::normalize(enemies[i]->getPosition() - position).x, glm::normalize(enemies[i]->getPosition() - position).y)) < PI / 3) {
					enemies[i]->addStagger(dTime * 2);
				}
			}
		}

	} else {
		//this pathing algorithm comes from enemy.cpp
		//while the path is not empty, change velocity to go move towards the next node
		if (path.size() > 0) {
			glm::vec3 playerToPath = glm::vec3(path.top()->getPosition().x - position.x, path.top()->getPosition().y - position.y, 0);
			float angle = glm::atan(playerToPath.y, playerToPath.x);
			velocity = glm::vec3(std::cos(angle), std::sin(angle), 0) * 1.0f;
			if (glm::length(playerToPath) < 0.1f) {
				path.pop();
				if (path.size() == 0) { velocity = glm::vec3(0, 0, 0); }
			}
		}
		else { exist = false; }
		MobileObject::update(dTime);
	}
	
	//when the timer has exaplsed delete the wizard
	if (timer > lifeTime) { exist = false; }

	MobileObject::update(dTime);
}

//this function comes from tower.cpp
Enemy *Wizard::findEnemyInRange(vector<Enemy*> enemies) {
	Enemy *target = nullptr;//this keeps the pointer to the closest enemy, it is the return value
	float targetDistance = numeric_limits<float>::max();//this keeps the distance to the closest enemy

	//iterate over enemies, calculate the distance between the enemy and this tower
	for (int i = 0; i < enemies.size(); i++) {
		float distance = glm::distance(position, enemies[i]->getPosition());
		//if distance less than range and the closest enemies distance, set target to this enemy and distance to this distance
		if (distance < WIZARDRANGE && distance < targetDistance) {
			targetDistance = distance;
			target = enemies[i];
		}
	}
	//return closest enemy
	return target;
}

void Wizard::render(Renderer *renderer)
{
	//setup transform matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);//translate by position
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);//scale by scale
	glm::mat4 transformationMatrix = translationMatrix * scaleMatrix;//combine transformations

	//render game object
	renderer->drawSprite(texture, transformationMatrix);

	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	//render particles if firing (if an enemy is in range), direction is the direction of the flame
	if (firing) { renderer->drawFire("fire", translationMatrix * scaleMatrix, direction, 0); }


}
