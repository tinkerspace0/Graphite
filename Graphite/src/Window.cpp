#include <iostream>

#include <glad/glad.h>

#include <Window.h>

namespace Graphite
{

	Window::Window(int width, int height, const std::string& title)
		: m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr) {}

	bool Window::Init()
	{
		if (!glfwInit())
		{
			std::cerr << "Failed to Initialize GLFW" << std::endl;
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// Set Major Context Version for GLFW
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// Set Minor Context Version for GLFW
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Tell GLFW to set an OpenGL Profile for window creation with OpenGL Core Profile

		// Create GLFW WIndow
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
		if (!m_Window) {
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_Window);	// Set the created window as the current context

		// Use GLAD Loader to load OpenGL states (Could be considered as an example of OpenGL initialization even though OpenGL is actually not a library or anything to initialize)
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize OpenGL" << std::endl;
			return false;
		}

		glfwSwapInterval(1);  // Enable vsync

		return true;
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Window::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	Window::~Window()
	{
		if (m_Window) {
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}
	}

}