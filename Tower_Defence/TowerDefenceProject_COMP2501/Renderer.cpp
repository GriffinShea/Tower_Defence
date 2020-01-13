
#include "Renderer.h"

Renderer::Renderer(const char *spriteShaderPath, const char *fireParticleShaderPath, const char* explosionParticleShaderPath, const char *fragmentShaderPath, const char *textVertShaderPath, const char *textFragShaderPath) {
	
	// Load vertex program source code
	string spriteShaderFile = FileUtils::LoadTextFile(spriteShaderPath);
	const char *source_spriteShaderFile = spriteShaderFile.c_str();

	string fireParticleShaderFile = FileUtils::LoadTextFile(fireParticleShaderPath);
	const char *source_fireParticleShaderFile = fireParticleShaderFile.c_str();
	
	string explosionParticleShaderFile = FileUtils::LoadTextFile(explosionParticleShaderPath);
	const char *source_explosionParticleShaderFile = explosionParticleShaderFile.c_str();
	
	string fragmentShaderFile = FileUtils::LoadTextFile(fragmentShaderPath);
	const char *source_fragmentShaderFile = fragmentShaderFile.c_str();
	
	string textVertShaderFile = FileUtils::LoadTextFile(textVertShaderPath);
	const char *source_textVertShaderFile = textVertShaderFile.c_str();
	
	string textFragShaderFile = FileUtils::LoadTextFile(textFragShaderPath);
	const char *source_textFragShaderFile = textFragShaderFile.c_str();

	//compile shaders
	GLuint spriteShader = compileVertexShader(source_spriteShaderFile);
	GLuint fireParticleShader = compileVertexShader(source_fireParticleShaderFile);
	GLuint explosionParticleShader = compileVertexShader(source_explosionParticleShaderFile);
	GLuint fragmentShader = compileFragmentShader(source_fragmentShaderFile);
	GLuint textVertShader = compileVertexShader(source_textVertShaderFile);
	GLuint textFragShader = compileFragmentShader(source_textFragShaderFile);

	//create programs
	spriteProgram = createShaderProgram(spriteShader, fragmentShader);
	fireParticleProgram = createShaderProgram(fireParticleShader, fragmentShader);
	explosionParticleProgram = createShaderProgram(explosionParticleShader, fragmentShader);
	textProgram = createShaderProgram(textVertShader, textFragShader);

	//cleanup
	glDeleteShader(spriteShader);
	glDeleteShader(fireParticleShader);
	glDeleteShader(explosionParticleShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(textVertShader);
	glDeleteShader(textFragShader);
	//setup sprite and particle geometry
	spriteSize = createSquare();
	particleSize = createParticleArray();
	//load textures
	loadTextures();

	return;
}

GLuint Renderer::compileVertexShader(const char *source) {
	// Create a shader from vertex program source code
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	// Check if shader compiled successfully
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		throw(ios_base::failure(string("Error compiling regular vertex shader: ") + string(buffer)));
	}
	return shader;
}

GLuint Renderer::compileFragmentShader(const char *source) {
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	// Check if shader compiled successfully
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		throw(ios_base::failure(string("Error compiling fragment shader: ") + string(buffer)));
	}
	return shader;
}

GLuint Renderer::createShaderProgram(GLuint vert, GLuint frag) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	// Check if shaders were linked successfully
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(program, 512, NULL, buffer);
		throw(ios_base::failure(string("Error linking regular shaders: ") + string(buffer)));
	}
	return program;
}

int Renderer::createSquare(void)
{
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
	//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
	//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &sprite_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &sprite_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer
	return sizeof(face);
}

int Renderer::createParticleArray(void) {

	// Each particle is a square with four vertices and two triangles

	// Number of attributes for vertices and faces
	const int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), direction (2), 2D texture coordinates (2), time (1)
	//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};

	GLfloat particleatt[1000 * vertex_attr];
	float theta, r, tmod;

	for (int i = 0; i < 1000; i++)
	{
		if (i % 4 == 0)
		{
			theta = (6.28*(rand() % 1000) / 1000.0f);//(2*(rand() % 10000) / 10000.0f -1.0f)*0.13f;
			r = 0.2f + 0.8*(rand() % 10000) / 10000.0f;
			tmod = (rand() % 10000) / 10000.0f;
		}
		// position	
		particleatt[i*vertex_attr + 0] = vertex[(i % 4) * 7 + 0];
		particleatt[i*vertex_attr + 1] = vertex[(i % 4) * 7 + 1];

		// velocity
		particleatt[i*vertex_attr + 2] = sin(theta)*r;
		particleatt[i*vertex_attr + 3] = cos(theta)*r;

		// phase
		particleatt[i*vertex_attr + 4] = tmod;

		// texture coordinate
		particleatt[i*vertex_attr + 5] = vertex[(i % 4) * 7 + 5];
		particleatt[i*vertex_attr + 6] = vertex[(i % 4) * 7 + 6];
	}

	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint manyface[1000 * 6];

	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 6; j++)
			manyface[i * 6 + j] = face[j] + i * 4;
	}

	// Create buffer for vertices
	glGenBuffers(1, &partic_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, partic_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleatt), particleatt, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &partic_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, partic_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyface), manyface, GL_STATIC_DRAW);

	// Return number of elements in array buffer
	return sizeof(manyface);
}

void Renderer::loadTextures() {
	glGenTextures(texturesNum, textures);
	loadTexture(textures[0], "assets/Title_Screen.png");
	textureIndexes["Title_Screen"] = 0;
	loadTexture(textures[1], "assets/maze_1.png");
	textureIndexes["maze_1"] = 1;
	loadTexture(textures[2], "assets/maze_2.png");
	textureIndexes["maze_2"] = 2;
	loadTexture(textures[3], "assets/maze_3.png");
	textureIndexes["maze_3"] = 3;

	loadTexture(textures[4], "assets/rock.png");
	textureIndexes["rock"] = 4;
	loadTexture(textures[5], "assets/Zombie.png");
	textureIndexes["Zombie"] = 5;
	loadTexture(textures[6], "assets/Skeleton.png");
	textureIndexes["Skeleton"] = 6;
	loadTexture(textures[7], "assets/Wizard.png");
	textureIndexes["Wizard"] = 7;
	loadTexture(textures[8], "assets/Necromancer.png");
	textureIndexes["Necromancer"] = 8;
	loadTexture(textures[9], "assets/FireTower.png");
	textureIndexes["FireTower"] = 9;
	loadTexture(textures[10], "assets/IceTower.png");
	textureIndexes["IceTower"] = 10;
	loadTexture(textures[11], "assets/LightningTower.png");
	textureIndexes["LightningTower"] = 11;
	loadTexture(textures[12], "assets/text.png");
	textureIndexes["text"] = 12;
	loadTexture(textures[13], "assets/fire.png");
	textureIndexes["fire"] = 13;
	loadTexture(textures[14], "assets/WizardPowerUp.png");
	textureIndexes["WizardPowerUp"] = 14;
	loadTexture(textures[15], "assets/lightning1.png");
	textureIndexes["lightning1"] = 15;
	loadTexture(textures[16], "assets/lightning2.png");
	textureIndexes["lightning2"] = 16;
	loadTexture(textures[17], "assets/ice.png");
	textureIndexes["ice"] = 17;
	loadTexture(textures[18], "assets/NO_TEXTURE.png");
	textureIndexes["NO_TEXTURE"] = 18;

	glBindTexture(GL_TEXTURE_2D, textures[0]);
}

void Renderer::loadTexture(GLuint w, string fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::drawSprite(string textureName, glm::mat4 &transformationMatrix) {
	enableSpriteProgram();
	if (!textureIndexes.count(textureName)) { cout << textureName << " <-- sprite texture not found\n"; }
	glBindTexture(GL_TEXTURE_2D, textures[textureIndexes.at(textureName)]);//bind texture
	glUniformMatrix4fv(glGetUniformLocation(*currProg, "transformationMatrix"), 1, GL_FALSE, glm::value_ptr(transformationMatrix));//set transformation uniform
	glDrawElements(GL_TRIANGLES, spriteSize, GL_UNSIGNED_INT, 0);//draw

}

void Renderer::drawFire(string textureName, glm::mat4 &transformationMatrixSource, glm::vec2 direction, float redness) {
	enableFireParticleProgram();
	if (!textureIndexes.count(textureName)) { cout << textureName << " <-- particle texture not found\n"; }
	transformationMatrixSource = glm::scale(transformationMatrixSource, glm::vec3(0.5, 0.5, 1));//scale down
	glUniformMatrix4fv(glGetUniformLocation(*currProg, "transformationMatrix"), 1, GL_FALSE, glm::value_ptr(transformationMatrixSource));//set transformation uniform
	glUniform1f(glGetUniformLocation(*currProg, "time"), glfwGetTime());//set time uniform
	glUniform1f(glGetUniformLocation(*currProg, "fireRedness"), redness);//set fire redness
	glUniform2f(glGetUniformLocation(*currProg, "direction"), direction.x, direction.y);//pass in the direction of the flame
	glBindTexture(GL_TEXTURE_2D, textures[textureIndexes.at(textureName)]);//bind texture
	glDrawElements(GL_TRIANGLES, 6 * particleSize, GL_UNSIGNED_INT, 0);// Draw

}

void Renderer::drawExplosion(string textureName, glm::mat4 &transformationMatrixSource, float timePassed) {
	enableExplosionParticleProgram();
	if (!textureIndexes.count(textureName)) { cout << textureName << " <-- particle texture not found\n"; }
	transformationMatrixSource = glm::scale(transformationMatrixSource, glm::vec3(0.3, 0.3, 1));//scale down
	glUniformMatrix4fv(glGetUniformLocation(*currProg, "transformationMatrix"), 1, GL_FALSE, glm::value_ptr(transformationMatrixSource));//set transformation uniform
	glUniform1f(glGetUniformLocation(*currProg, "time"), timePassed);//set time uniform
	glBindTexture(GL_TEXTURE_2D, textures[textureIndexes.at(textureName)]);//bind texture
	glDrawElements(GL_TRIANGLES, 6 * particleSize, GL_UNSIGNED_INT, 0);// Draw

}

void Renderer::renderText(glm::vec3 textPosition, float spacing, string &stringToRender) {
	// Enable the shader and bind the proper text spritesheet texture
	enableTextProgram();
	glBindTexture(GL_TEXTURE_2D, textures[textureIndexes.at("text")]);

	// Loop through each character and draw it
	for (int i = 0; i < stringToRender.size(); i++) {
		// We need to get the character and map it to a UV coordinate the represents where it is located in the text spritesheet texture
		// First get the character and make sure it is an upper case character (less cases to cover)
		//int ascii = (int)toupper(stringToRender[i], locale());
		int ascii = (int)toupper(stringToRender[i]);

		// Convert our ascii value into the range of [0, 35] (0, 1, 2, ... , A, B, C, .... , Z)
		if (ascii > 57)
			ascii -= 7;

		ascii -= 48;

		// Get the row and column of the character in our sprite sheet. Then we will let our vertex shader calculate the proper UVs
		int spritesheetSize = 6;
		float charUVSize = 1.0f / spritesheetSize;
		int charRow = ascii / spritesheetSize;
		int charCol = ascii % spritesheetSize;

		// Before we draw, we need to setup the transformation matrix for the text (doesn't use a view matrix)
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(textPosition.x + (spacing * i), textPosition.y, 0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.08));
		glm::mat4 transformationMatrix = translation * scale;

		// Setup uniforms
		glUniform1f(glGetUniformLocation(*currProg, "UVSize"), charUVSize);
		glUniform1i(glGetUniformLocation(*currProg, "charCol"), charCol);
		glUniform1i(glGetUniformLocation(*currProg, "charRow"), charRow);
		glUniformMatrix4fv(glGetUniformLocation(*currProg, "transformationMatrix"), 1, GL_FALSE, glm::value_ptr(transformationMatrix));
		glUniform3f(glGetUniformLocation(*currProg, "textColour"), 1.0f, 0.0f, 0.0f);

		//textShader->setUniform1f("UVSize", charUVSize);
		//textShader->setUniform1i(, charCol);
		//textShader->setUniform1i(, charRow);
		//textShader->setUniformMat4("transformationMatrix", transformationMatrix);
		//textShader->setUniform3f("textColour", glm::vec3(1.0f, 0.0f, 0.0f));

		// Finally draw the character
		glDrawElements(GL_TRIANGLES, spritesheetSize, GL_UNSIGNED_INT, 0);
	}
}

//enables regular program
void Renderer::enableSpriteProgram() {
	//check if this shader is already active
	if (currProg == &spriteProgram) { return; }
	//enable regular shader program
	glUseProgram(spriteProgram);
	//bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
	//bind attributes
	GLint vertex_att = glGetAttribLocation(spriteProgram, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);
	GLint color_att = glGetAttribLocation(spriteProgram, "color");
	glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(color_att);
	GLint tex_att = glGetAttribLocation(spriteProgram, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);
	//set current program
	currProg = &spriteProgram;

}

//enables particle program
void Renderer::enableFireParticleProgram() {
	//check if this shader is already active
	if (currProg == &fireParticleProgram) { return; }
	//enable particle shader program
	glUseProgram(fireParticleProgram);
	//bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, partic_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, partic_ebo);
	//bind attributes
	GLint vertex_att = glGetAttribLocation(fireParticleProgram, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);
	GLint dir_att = glGetAttribLocation(fireParticleProgram, "dir");
	glVertexAttribPointer(dir_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(dir_att);
	GLint time_att = glGetAttribLocation(fireParticleProgram, "t");
	glVertexAttribPointer(time_att, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(time_att);
	GLint tex_att = glGetAttribLocation(fireParticleProgram, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);
	//set current program
	currProg = &fireParticleProgram;
}

// enables lightning program
void Renderer::enableExplosionParticleProgram()
{
	//check if this shader is already active
	if (currProg == &explosionParticleProgram) { return; }
	//enable particle shader program
	glUseProgram(explosionParticleProgram);
	//bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, partic_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, partic_ebo);
	//bind attributes
	GLint vertex_att = glGetAttribLocation(explosionParticleProgram, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);
	GLint dir_att = glGetAttribLocation(explosionParticleProgram, "dir");
	glVertexAttribPointer(dir_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(dir_att);
	GLint time_att = glGetAttribLocation(explosionParticleProgram, "t");
	glVertexAttribPointer(time_att, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(time_att);
	GLint tex_att = glGetAttribLocation(explosionParticleProgram, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);
	//set current program
	currProg = &explosionParticleProgram;
}

//enables text program
void Renderer::enableTextProgram() {
	//check if this shader is already active
	if (currProg == &textProgram) { return; }
	//enable TEXT shader program
	glUseProgram(textProgram);
	//bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
	//bind attributes
	GLint vertex_att = glGetAttribLocation(textProgram, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);
	GLint color_att = glGetAttribLocation(textProgram, "color");
	glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(color_att);
	GLint tex_att = glGetAttribLocation(textProgram, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);
	//set current program
	currProg = &textProgram;

}

void Renderer::disable() {
	glUseProgram(0);
	currProg = nullptr;
}

Renderer::~Renderer() {
	glDeleteProgram(spriteProgram);
	glDeleteProgram(fireParticleProgram);
	glDeleteProgram(explosionParticleProgram);
	glDeleteProgram(textProgram);
}
