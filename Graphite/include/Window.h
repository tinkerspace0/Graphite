#pragma once

#include <string>

#include <GLFW/glfw3.h>

namespace Graphite
{
	class Window
	{
	public:
		Window(int width, int height, const std::string& title);

		bool Init();	// Initialize the window and context
		void PollEvents();	// Poll for window events
		void SwapBuffers();	// Swaps the front and back buffers
		bool ShouldClose() const;	// Return true if the window should close
		void SetClearColor(float r, float g, float b, float a);	// Sets the clear color
		void Clear();
		~Window();

	private:
		int m_Width, m_Height;
		std::string m_Title;
		GLFWwindow* m_Window;
	};
}