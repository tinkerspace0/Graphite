#pragma once

#include "Graphite/Core/Base.h"

#include "Graphite/Core/Window.h"
#include "Graphite/Core/LayerStack.h"
#include "Graphite/Events/Event.h"
#include "Graphite/Events/ApplicationEvent.h"

#include "Graphite/Core/Timestep.h"

#include "Graphite/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Graphite {

	class Application
	{
	public:
		Application(const std::string& name = "Graphite App");
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}