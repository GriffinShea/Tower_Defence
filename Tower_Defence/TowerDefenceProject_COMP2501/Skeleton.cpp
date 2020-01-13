#include "Skeleton.h"

//NOTE: textures could be stored somewhere outside the game object and be pulled directly from that place
//scale will be dependant on texture
Skeleton::Skeleton(float s, stack<Node*> p, glm::vec3 ps)
	: Enemy(s, p, SKELETONHEALTH, SKELETONLOOT, SKELETONSPEED, ps, SKELETONSCALE) {
	texture = "Skeleton";

}
