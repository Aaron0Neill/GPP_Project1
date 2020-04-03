#include "GameObject.h"

GLuint GameObject::m_totalModelNumber{ 0 };

GameObject::GameObject(glm::vec3 t_startPos) :
	m_model{ glm::mat4(1.0f) },
	m_position{ glm::vec3() },
	m_startPos{ t_startPos }
{
	// copy the cube info into the game object
	memcpy(this->m_vertices, vertices, sizeof(GLfloat) * VERTICES * 3);
	memcpy(this->m_colors, colors, sizeof(GLfloat) * COLORS * 4);
	memcpy(this->m_uvs, uvs, sizeof(GLfloat) * UVS);
	memcpy(this->m_indices, indices, sizeof(GLfloat) * INDICES * 3);

	m_modelIndex = m_totalModelNumber;
	m_totalModelNumber++;

	m_gravity = glm::vec3{ 0,.0598 * m_numberScalar,0 };
	m_startVelocity = glm::vec3(10 * m_numberScalar, 0, 0);
	m_rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);

	m_velocity = m_startVelocity;

	setPosition(m_startPos);

	m_rotationSpeed = 4.0f * m_numberScalar;

	if (m_modelIndex % 2 == 0)
	{
		m_rotationSpeed = -m_rotationSpeed;
	}

	//GLfloat allUVS[] = {

	//		0.0, 0.0,
	//		1.0, 0.0,
	//		1.0, 1.0,
	//		0.0, 1.0
	//};
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
	m_position += t_movementVec;
	m_model = glm::translate(m_model, t_movementVec);
}

void GameObject::setPosition(glm::vec3 t_position)
{
	m_position = t_position;
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, m_position);
}

void GameObject::setVelocity(glm::vec3 t_velocity)
{
	m_velocity = t_velocity * m_numberScalar;
}

void GameObject::setScale(float t_scale)
{
	for (int i = 0; i < VERTICES * 3; i++)
	{
		m_vertices[i] *= t_scale;
	}
}

void GameObject::update()
{
	if (m_currentState == CubeState::alive)
	{
		if (m_position.x > 14)
		{
			setPosition(m_startPos);
		}
		if (m_position.y < -4)
		{
			m_velocity.y = 75.0f * m_numberScalar;
			m_rotationSpeed = -m_rotationSpeed;
		}
		rotate(glm::vec3(1, 0, 0));
	}
	else if (m_currentState == CubeState::dead)
	{
		if (m_position.y < -10)
		{
			setPosition(m_startPos);
			m_velocity = m_startVelocity;
			m_currentState = CubeState::alive;
		}
	}

	m_velocity -= m_gravity;

	translate(m_velocity);
}