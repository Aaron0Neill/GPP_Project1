#ifndef GAME_H
#define GAME_H

#include <string>
#include <sstream>

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Graphics.hpp>
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

const unsigned GAME_OBJECTS{ 12 };

const unsigned GROUND_OBJECTS{ 5 };


const unsigned MAX_CUBES{ GAME_OBJECTS + GROUND_OBJECTS };

class Game
{
public:
	Game();
	Game(sf::ContextSettings settings);
	~Game();
	void run();

private:

	bool m_jumping{ true };
	GameObject* m_enemyObjects[GAME_OBJECTS];
	RenderWindow window;
	bool isRunning = false;
	void initialize();
	void update();
	void render();
	void renderGround(GameObject* obj);
	void unload();

	float vectorLength(sf::Vector2f t_vec);

	sf::Vector2f calculateScreenPos(glm::vec3 t_pos);

	vec3 m_gravity;
	vec3 m_velocity;
	float m_numberScalar{ 0.00005 };

	vec3 m_gameObjectStartPos;

	float mouseRadius;

	float cubeRadius;

	float totalRadius;

	GameObject* m_groundObjects[GROUND_OBJECTS];

	void drawCube(GameObject* obj);
};

#endif