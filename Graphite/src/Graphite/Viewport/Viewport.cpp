#include "gfpch.h"
#include "Graphite/Viewport/Viewport.h"
#include <imgui.h>
#include "glm/gtc/matrix_transform.hpp"

namespace Graphite
{
	Viewport::Viewport(const std::string& viewportName, uint32_t width, uint32_t height, bool initialize)
		: m_Name(viewportName)
	{
		m_fbSpec.Width = width;
		m_fbSpec.Height = height;
		if (initialize)
			init();
	}

	Viewport::~Viewport() {}

	void Viewport::init() {
		// Set up Vertex Array for Square
		m_SquareVA = Graphite::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Load textures and shaders
		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);

		// Set up framebuffer
		if (!m_Initialized) {
			m_Framebuffer = Framebuffer::Create(m_fbSpec);
			m_Initialized = true;
		}
	}

	void Viewport::Resize(uint32_t width, uint32_t height) {
		if (m_Framebuffer && (width != m_fbSpec.Width || height != m_fbSpec.Height)) {
			m_fbSpec.Width = width;
			m_fbSpec.Height = height;
			m_Framebuffer->Resize(width, height);
		}
	}

	void Viewport::renderViewport() {
		// ImGui window setup
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != glm::vec2(viewportPanelSize.x, viewportPanelSize.y)) {
			Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		// Render to framebuffer
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		auto textureShader = m_ShaderLibrary.Get("Texture");
		textureShader->Bind();
		textureShader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
		textureShader->SetFloat("u_TilingFactor", 1.0f);

		// Render the quad with the texture
		m_ChernoLogoTexture->Bind(0);
		Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_Framebuffer->Unbind();

		// Display framebuffer in ImGui
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Viewport::process()
	{

	}

}
