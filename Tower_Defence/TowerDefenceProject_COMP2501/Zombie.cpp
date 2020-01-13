#include "Zombie.h"

//NOTE: textures could be stored somewhere outside the game object and be pulled directly from that place
//scale will be dependant on texture
Zombie::Zombie(float s, stack<Node*> p, glm::vec3 ps)
	: Enemy(s, p, ZOMBIEHEALTH, ZOMBIELOOT, ZOMBIESPEED, ps, ZOMBIESCALE) {
	texture = "Zombie";
}
