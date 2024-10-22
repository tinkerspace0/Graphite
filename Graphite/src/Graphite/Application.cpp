#include "gfpch.h"
#include "Application.h"

#include "Graphite/Events/KeyEvent.h"
#include "Graphite/Log.h"

namespace Graphite
{
	Application::Application() {}

	Application::~Application() {}

	void Application::Run()
	{
		KeyPresesdEvent e(5, 3);
		if (e.IsInCategory(EventCategoryKeyboard))
			GF_CORE_TRACE(e.ToString());
		else
			GF_CORE_TRACE("Not Mouse Event");
		while (true);
	}
}