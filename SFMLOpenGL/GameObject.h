#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <stdio.h>
#include "Cube.h"
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CubeState
{
	idle,
	alive,
	dead
};


class GameObject
{
public:
	GameObject(glm::vec3 t_startPos);
	~GameObject();

	GLuint const getModelNum() { return m_modelIndex; }

	void rotate(glm::vec3 t_axis);

	void translate(glm::vec3 t_movementVec);

	glm::vec3 const getPositon() { return m_position; }

	void setPosition(glm::vec3 t_position);

	void setState(CubeState t_state) { m_currentState = t_state; };

	void setVelocity(glm::vec3 t_velocity);

	void setScale(float t_scale);

	GLfloat* getVertex() { return &m_vertices[0]; }

	GLfloat* getIndices() { return &m_indices[0]; }

	GLfloat* getUVS() { return &m_uvs[0]; }

	GLfloat* getColour() { return &m_colors[0]; }

	glm::mat4 getModel() { return  m_model; }

	void update();

private:

	sf::CircleShape m_hitBox;

	CubeState m_currentState;

	float m_rotationSpeed;

	glm::vec3 m_startVelocity;
	glm::vec3 m_startPos;
	glm::vec3 m_gravity;
	glm::vec3 m_velocity;
	glm::vec3 m_rotationAxis;

	float m_numberScalar{ 0.00005 };

	static GLuint m_totalModelNumber;

	GLuint m_modelIndex;

	GLfloat m_vertices[VERTICES * 3];

	GLfloat m_colors[COLORS * 4];

	GLfloat m_uvs[UVS];

	GLfloat m_indices[INDICES * 3];

	glm::vec3 m_position;

	glm::mat4 m_model;
};

#endif // !GAME_OBJECT_H