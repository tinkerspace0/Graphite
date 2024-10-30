#include "gfpch.h"
#include "Graphite/Core/Window.h"

#ifdef GF_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Graphite
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef GF_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		GF_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}