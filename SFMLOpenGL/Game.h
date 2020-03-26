#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <Debug.h>

#include <fstream>
#include <istream>

#include <GameObject.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace sf;
using namespace glm;

const unsigned MAX_CUBES{ 2 };

class Game
{
public:
	Game();
	Game(sf::ContextSettings settings);
	~Game();
	void run();

private:
	GameObject* m_enemyObjects[MAX_CUBES];
	Window window;
	bool isRunning = false;
	void initialize();
	void update();
	void render();
	void unload();

	void drawCube(mat4 mvp, GameObject* obj);
	void checkInput();
};

#endif