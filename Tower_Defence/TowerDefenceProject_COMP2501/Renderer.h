
#ifndef RENDERER_H
#define RENDERER_H


#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h> // read image file
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glfw3.h>
#include <map>

#include "FileUtils.h"

using namespace std;

static const int texturesNum = 20;

class Renderer {
public:
	Renderer(const char*, const char*, const char*, const char*, const char*, const char*);
	~Renderer();

	int createSquare(void);
	int createParticleArray(void);
	void loadTextures();
	void loadTexture(GLuint, string);

	void drawSprite(string, glm::mat4&);
	void drawFire(string, glm::mat4&, glm::vec2, float);
	void drawExplosion(string, glm::mat4&, float);
	void renderText(glm::vec3, float, string&);

	void enableSpriteProgram();
	void enableFireParticleProgram();
	void enableExplosionParticleProgram();
	void enableTextProgram();

	void disable();

private:

	GLuint compileVertexShader(const char*);
	GLuint compileFragmentShader(const char*);
	GLuint createShaderProgram(GLuint, GLuint);

	//shader program currently being used
	GLuint *currProg;

	//sprite and text shader program, vbo, ebo, and size
	GLuint spriteProgram;
	GLuint textProgram;
	GLuint sprite_vbo;
	GLuint sprite_ebo;

	int spriteSize;
	//partical shader programs, vbo, ebo, and size
	GLuint fireParticleProgram;
	GLuint explosionParticleProgram;
	GLuint partic_vbo;
	GLuint partic_ebo;
	int particleSize;

	//textureIndexes is a map from texture names to the indexes of textures in textures
	map<string, int> textureIndexes;
	GLuint textures[texturesNum];
};
#endif
