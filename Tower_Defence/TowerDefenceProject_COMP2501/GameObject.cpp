#include "GameObject.h"

GameObject::GameObject(glm::vec3 p, glm::vec3 s) {
	exist = true;
	position = p;
	scale = s;
	texture = "NO_TEXTURE";
	return;
}

void GameObject::render(Renderer *renderer) {
	//render game object
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);//translate by position
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);//scale by scale
	glm::mat4 transformationMatrix = translationMatrix * scaleMatrix;//combine transformations
	renderer->drawSprite(texture, transformationMatrix);//draw
	return;
}

float GameObject::distanceToOther(GameObject o) { return glm::distance(position, o.position); }
