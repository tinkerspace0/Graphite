#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Graphite/Events/ApplicationEvent.h"


namespace Graphite
{
	class GRAPHITE_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
		
		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be Defined by client
	Application* CreateApplication();
}