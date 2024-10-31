#include "gfpch.h"
#include "Graphite/Viewport/Viewport.h"
#include <imgui.h>
#include "glm/gtc/matrix_transform.hpp"

#include "Graphite/Renderer/Renderer2D.h"

namespace Graphite
{
	Viewport::Viewport(const std::string& viewportName, uint32_t width, uint32_t height, bool initialize)
		: m_Name(viewportName), m_ViewportSize({ width, height }), m_CameraController(width / float(height), true)
	{
		// Define framebuffer specification with color format
		m_fbSpec.Width = width;
		m_fbSpec.Height = height;
		m_fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };

		if (initialize)
			init();
	}

	Viewport::~Viewport() {}

	void Viewport::init() {
		////////////////////////////////////////////////////////////////////////////
		//////////////////Do Not Modify the code between this section///////////////
		////////////////////////////////////////////////////////////////////////////

		// Set up framebuffer
		if (!m_Initialized) {
			m_Framebuffer = Framebuffer::Create(m_fbSpec);
			m_Initialized = true;
		}

		// Set up Vertex Array for rendering a textured quad
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

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);

		////////////////////////////////////////////////////////////////////////////
		//////////////////Do Not Modify the code above this section/////////////////
		////////////////////////////////////////////////////////////////////////////

		// Load textures and shaders
		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");
	}

	void Viewport::Resize(uint32_t width, uint32_t height) {
		if (m_Framebuffer && (width != m_fbSpec.Width || height != m_fbSpec.Height)) {
			m_fbSpec.Width = width;
			m_fbSpec.Height = height;
			m_Framebuffer->Resize(width, height);
		}
	}

	void Viewport::OnImGuiRender() {
		////////////////////////////////////////////////////////////////////////////
		//////////////////Do Not Modify the code between this section///////////////
		////////////////////////////////////////////////////////////////////////////

		// ImGui window setup for displaying the framebuffer texture
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != glm::vec2(viewportPanelSize.x, viewportPanelSize.y)) {
			Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		// Display framebuffer texture in ImGui
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		GF_CORE_INFO("Texture ID: {0}", textureID);
		ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		////////////////////////////////////////////////////////////////////////////
		//////////////////Do Not Modify the code above this section/////////////////
		////////////////////////////////////////////////////////////////////////////
	}

	void Viewport::Render(Timestep ts) {

		m_CameraController.OnUpdate(ts);

		// Render scene to framebuffer
		m_Framebuffer->Bind();

		Renderer::BeginScene(m_CameraController.GetCamera());

		RenderCommand::SetClearColor({ 0.8f, 0.5f, 0.2f, 1.0f });
		RenderCommand::Clear();

		auto textureShader = m_ShaderLibrary.Get("Texture");
		textureShader->Bind();
		textureShader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
		textureShader->SetFloat("u_TilingFactor", 1.0f);

		// Render Logic here



		// Render Logic above here

		m_Texture->Bind(0);
		Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

}
