#pragma once

#include "gfpch.h"
#include "Graphite/Renderer/Framebuffer.h"
#include "Graphite/Renderer/VertexArray.h"
#include "Graphite/Renderer/Buffer.h"
#include "Graphite/Renderer/Shader.h"
#include "Graphite/Renderer/Texture.h"
#include "Graphite/Renderer/Renderer.h"

#include "Graphite/Renderer/OrthographicCameraController.h"
#include "Graphite/Renderer/CameraController.h"

#include "glm/glm.hpp"

namespace Graphite
{
	class Viewport
	{
	public:
		Viewport(const std::string& viewportName, uint32_t width = 1280, uint32_t height = 720, bool initialize = true);
		~Viewport();

		void init();
		void Resize(uint32_t width, uint32_t height);
		void OnImGuiRender();
		void Render(Timestep ts);
		void OnEvent(Event& e);

	private:
		std::string m_Name;
		glm::vec2 m_ViewportSize;
		Ref<Framebuffer> m_Framebuffer;
		FramebufferSpecification m_fbSpec;
		bool m_Initialized = false;

		// Rendering assets
		Graphite::ShaderLibrary m_ShaderLibrary;
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Texture2D> m_Texture, m_ChernoLogoTexture;

		// Viewport flags
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		CameraController m_CameraController;
		glm::vec4 m_SquareColor = { 0.8f, 0.3f, 0.8f, 0.9f };

	private:
		// Grid settings
		float gridSpacing = 1.0f;      // Distance between lines
		int gridLineCount = 20;        // Number of lines in each direction
		float gridSize = gridLineCount * gridSpacing;
		glm::vec4 gridColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f); // Light gray color for grid

	};
}
