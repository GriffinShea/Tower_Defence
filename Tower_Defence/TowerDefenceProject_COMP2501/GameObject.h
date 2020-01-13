#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "Renderer.h"

const long double PI = 3.141592653589793238L;

using namespace std;

class GameObject {
	public:
		GameObject(glm::vec3, glm::vec3);

		//render to screen using shader
		virtual void render(Renderer*);
		//calculates distance to other GameObject
		float distanceToOther(GameObject);

		inline void setExistance(bool e) { exist = e; }

		inline bool exists() { return exist; }
		inline glm::vec3 getPosition() { return position; }
		inline glm::vec3 getScale() { return scale; }

	protected:
		bool exist;
		glm::vec3 position;
		glm::vec3 scale;
		string texture;

};

#endif