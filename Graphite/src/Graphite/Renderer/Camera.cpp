#include "gfpch.h"
#include "Graphite/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Graphite {

	Camera::Camera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		GF_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetProjection(float left, float right, float bottom, float top)
	{
		GF_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		GF_PROFILE_FUNCTION();
		// Create individual rotation matrices for yaw, pitch, and roll
		glm::mat4 rotationYaw = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotationPitch = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotationRoll = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));

		// Combine rotations in the order: roll -> pitch -> yaw
		glm::mat4 rotation = rotationYaw * rotationPitch * rotationRoll;

		// Combine rotation and translation
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * rotation;

		// Invert to get view matrix
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}