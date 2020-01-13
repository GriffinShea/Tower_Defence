#include "Necromancer.h"

//NOTE: textures could be stored somewhere outside the game object and be pulled directly from that place
//scale will be dependant on texture
Necromancer::Necromancer(float s, stack<Node*> p, glm::vec3 ps)
	: Enemy(s, p, NECROMANCERHEALTH, NECROMANCERLOOT, NECROMANCERSPEED, ps, NECROMANCERSCALE) {
	texture = "Necromancer";
}
