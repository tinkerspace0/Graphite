#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Graphite/Events/ApplicationEvent.h"
#include "Graphite/LayerStack.h"

namespace Graphite
{
	class GRAPHITE_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
		
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be Defined by client
	Application* CreateApplication();
}