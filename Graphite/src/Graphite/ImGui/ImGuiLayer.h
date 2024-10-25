#pragma once

#include "Graphite/Layer.h"
#include "Graphite/Events/KeyEvent.h"
#include "Graphite/Events/MouseEvent.h"
#include "Graphite/Events/ApplicationEvent.h"
#include "ImGuiWindow.h"

namespace Graphite
{
	class GRAPHITE_API ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnBegin();
		void SetupDock();
		void OnEnd();
		void OnEvent(Event& event);
		void AddImGuiWindow(ImGuiWindow* window);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPresesdEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;
		std::vector<ImGuiWindow*> m_Windows;  // List of windows
	};		

}