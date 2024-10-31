#pragma once

#include "gfpch.h"
#include "Graphite/Renderer/Framebuffer.h"
#include "Graphite/Renderer/VertexArray.h"
#include "Graphite/Renderer/Buffer.h"
#include "Graphite/Renderer/Shader.h"
#include "Graphite/Renderer/Texture.h"
#include "Graphite/Renderer/Renderer.h"

#include "Graphite/Renderer/OrthographicCameraController.h"

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
		OrthographicCameraController m_CameraController;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 0.1f };
	};
}
