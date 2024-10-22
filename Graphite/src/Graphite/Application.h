#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Graphite
{
	class GRAPHITE_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	};

	// To be Defined by client
	Application* CreateApplication();
}