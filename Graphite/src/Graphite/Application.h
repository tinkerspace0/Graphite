#pragma once

#include "Core.h"

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