#include "GameObject.h"

GameObject::GameObject()
{
	// copy the cube info into the game object
	memcpy(this->m_vertices, vertices, sizeof(GLfloat) * VERTICES * 3);
	memcpy(this->m_colors, colors, sizeof(GLfloat) * COLORS * 4);
	memcpy(this->m_uvs, uvs, sizeof(GLfloat) * UVS);
	memcpy(this->m_indices, indices, sizeof(GLfloat) * INDICES * 3);
	
	//GLfloat allUVS[] = {

	//		0.0, 0.0,
	//		1.0, 0.0,
	//		1.0, 1.0,
	//		0.0, 1.0
	//};

	//memcpy(m_uvs, allUVS, sizeof(GLfloat) * UVS);

	m_model = glm::mat4(1.0f);
}

GameObject::~GameObject()
{
}

void GameObject::rotate(glm::vec3 t_axis)
{
	m_model = glm::rotate(m_model, m_rotationSpeed, t_axis);
}

void GameObject::translate(glm::vec3 t_movementVec)
{
	setPosition(t_movementVec);
}

void GameObject::setPosition(glm::vec3 t_position)
{
	m_position = t_position;
	m_model = glm::translate(m_model, m_position);
}
