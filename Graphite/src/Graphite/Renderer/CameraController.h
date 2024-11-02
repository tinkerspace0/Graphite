#pragma once

#include "Camera.h"
#include "Graphite/Core/Timestep.h"
#include "Graphite/Events/ApplicationEvent.h"
#include "Graphite/Events/MouseEvent.h"

namespace Graphite {

	class CameraController
	{
	public:
		CameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		Camera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f }; //In degrees
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};
}