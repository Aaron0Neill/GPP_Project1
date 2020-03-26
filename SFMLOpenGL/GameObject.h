#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <stdio.h>
#include "Cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void rotate(glm::vec3 t_axis);

	void translate(glm::vec3 t_movementVec);

	glm::vec3 const getPositon() { return m_position; }

	/// <summary>
	/// @brief sets the current position of the game object
	/// </summary>
	/// <param name="t_position"> the position to set the cube to </param>
	void setPosition(glm::vec3 t_position);

	GLfloat* getVertex() { return &m_vertices[0]; }

	GLfloat* getIndices() { return &m_indices[0]; }

	GLfloat* getUVS() { return &m_uvs[0]; }

	GLfloat* getColour() { return &m_colors[0]; }

	glm::mat4 getModel() { return  m_model; }

private:

	float m_rotationSpeed{ 0.001f };

	GLfloat m_vertices[VERTICES * 3];

	GLfloat m_colors[COLORS * 4];

	GLfloat m_uvs[UVS];

	GLfloat m_indices[INDICES * 3];

	glm::vec3 m_position;

	glm::mat4 m_model;
};

#endif // !GAME_OBJECT_H