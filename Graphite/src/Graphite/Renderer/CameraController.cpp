#include "gfpch.h"
#include "CameraController.h"

#include "Graphite/Core/Input.h"
#include "Graphite/Core/KeyCodes.h"

namespace Graphite {

	CameraController::CameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation) {}

	void CameraController::OnUpdate(Timestep ts)
	{
		GF_PROFILE_FUNCTION();

		// Calculate forward and right vectors based on yaw rotation
		glm::vec3 forward = {
			-sin(glm::radians(m_CameraRotation.y)),
			cos(glm::radians(m_CameraRotation.y)),
			0.0f
		};
		glm::vec3 right = { forward.y, -forward.x, 0.0f }; // Perpendicular to forward

		// Camera movement (WASD keys)
		if (Input::IsKeyPressed(GF_KEY_A))
			m_CameraPosition -= right * m_CameraTranslationSpeed * static_cast<float>(ts);
		if (Input::IsKeyPressed(GF_KEY_D))
			m_CameraPosition += right * m_CameraTranslationSpeed * static_cast<float>(ts);
		if (Input::IsKeyPressed(GF_KEY_W))
			m_CameraPosition += forward * m_CameraTranslationSpeed * static_cast<float>(ts);
		if (Input::IsKeyPressed(GF_KEY_S))
			m_CameraPosition -= forward * m_CameraTranslationSpeed * static_cast<float>(ts);

		// Handle rotation if enabled
		if (m_Rotation)
		{
			// Roll (T and U keys)
			if (Input::IsKeyPressed(GF_KEY_T))
				m_CameraRotation.z += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(GF_KEY_U))
				m_CameraRotation.z -= m_CameraRotationSpeed * ts;

			// Pitch (Q and E keys)
			if (Input::IsKeyPressed(GF_KEY_Q))
				m_CameraRotation.x += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(GF_KEY_E))
				m_CameraRotation.x -= m_CameraRotationSpeed * ts;

			// Yaw (R and Y keys)
			if (Input::IsKeyPressed(GF_KEY_R))
				m_CameraRotation.y += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(GF_KEY_Y))
				m_CameraRotation.y -= m_CameraRotationSpeed * ts;

			// Clamp pitch to avoid flipping
			m_CameraRotation.x = glm::clamp(m_CameraRotation.x, -89.0f, 89.0f);

			// Normalize yaw to stay within -180 to +180 degrees
			if (m_CameraRotation.y > 180.0f)
				m_CameraRotation.y -= 360.0f;
			else if (m_CameraRotation.y <= -180.0f)
				m_CameraRotation.y += 360.0f;

			// Update the camera rotation
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void CameraController::OnEvent(Event& e)
	{
		GF_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GF_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GF_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	void CameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		GF_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		GF_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}