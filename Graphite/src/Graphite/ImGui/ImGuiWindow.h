#pragma once

class GRAPHITE_API ImGuiWindow
{
public:
	virtual ~ImGuiWindow() = default;
	virtual void Render() = 0;  // Override this in each custom window
};
