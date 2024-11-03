#pragma once

#include "gfpch.h"
#include "Graphite/Renderer/Framebuffer.h"
#include "Graphite/Renderer/VertexArray.h"
#include "Graphite/Renderer/Buffer.h"
#include "Graphite/Renderer/Shader.h"
#include "Graphite/Renderer/Texture.h"
#include "Graphite/Renderer/Renderer.h"

#include "Graphite/Viewport/ViewportCamera.h"
#include "Graphite/Renderer/OrthographicCameraController.h"

#include "glm/glm.hpp"



namespace Graphite
{
	class Viewport
	{
	public:
		Viewport(const std::string& viewportName, uint32_t width = 1280, uint32_t height = 720, bool initialize = true);
		~Viewport();

		void Resize(uint32_t width, uint32_t height);
		void OnImGuiRender();
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);


	private:
		std::string m_Name;
		glm::vec2 m_ViewportSize;
		Ref<Framebuffer> m_Framebuffer;
		FramebufferSpecification m_fbSpec;
		bool m_Initialized = false;

		// Viewport flags
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		bool m_ViewportRerender = true;
		ViewportCamera m_Camera;
		uint32_t m_ViewporttextureID;

	private:
		// Grid settings
		glm::vec4 gridColor = glm::vec4(0.85f, 0.8f, 0.7f, 1.0f); // Light gray color for grid

	};
}
