#pragma once

#include "Graphite/Core/Base.h"
#include "Graphite/Core/KeyCodes.h"
#include "Graphite/Core/MouseCodes.h"

namespace Graphite {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}