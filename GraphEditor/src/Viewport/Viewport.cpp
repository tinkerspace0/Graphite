#include "Viewport.h"
#include <imgui/imgui.h>
#include "glm/gtc/matrix_transform.hpp"


namespace Graphite
{
	Viewport::Viewport(const std::string& viewportName, uint32_t width, uint32_t height, bool initialize)
		: m_Name(viewportName), m_ViewportSize({ width, height })
	{
		// Define framebuffer specification with color format
		m_fbSpec.Width = width;
		m_fbSpec.Height = height;
		m_fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };
		m_Framebuffer = Framebuffer::Create(m_fbSpec);
		m_Initialized = true;
		m_ViewporttextureID = m_Framebuffer->GetColorAttachmentRendererID();
		SceneRenderer::Init();
	}

	Viewport::~Viewport() {}

	void Viewport::Resize(uint32_t width, uint32_t height) {
		if (m_Framebuffer && (width != m_fbSpec.Width || height != m_fbSpec.Height)) {
			m_fbSpec.Width = width;
			m_fbSpec.Height = height;
			m_Framebuffer->Resize(width, height);
			m_Camera.SetViewportSize(width, height);
			m_ViewportRerender = true;
		}
	}

	void Viewport::OnImGuiRender() {
		////////////////////////////////////////////////////////////////////////////
		//////////////////Do Not Modify the code between this section///////////////
		////////////////////////////////////////////////////////////////////////////

		// ImGui window setup for displaying the framebuffer texture
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(m_Name.c_str());

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != glm::vec2(viewportSize.x, viewportSize.y)) {
			Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_ViewportSize = { viewportSize.x, viewportSize.y };
		}

		
		// Display framebuffer texture in ImGui
		ImGui::Image((ImTextureID)(intptr_t)m_ViewporttextureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		////////////////////////////////////////////////////////////////////////////
		//////////////////Do Not Modify the code above this section/////////////////
		////////////////////////////////////////////////////////////////////////////
	}

	void Viewport::OnUpdate(const Ref<Scene>& scene, Timestep ts) {
		
		if (m_ViewportHovered && m_ViewportFocused)
		{
			m_Camera.OnUpdate(ts);

			// Render scene to framebuffer
			m_Framebuffer->Bind();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			SceneRenderer::BeginScene(m_Camera);
			//SceneRenderer::DrawGrid(m_Camera, 1.0f, 1000, { 0.2f, 0.2f, 0.2f, 1.0f });
			//SceneRenderer::DrawLine({ 0.0f, -100.0f, 0.0f }, { 0.0f, 100.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
			// Render Logic here

			SceneRenderer::DrawRect({ 1.0f, 0.8f, 10.0f }, { 3.0f, 2.0f }, { 30.0f, 20.0f, 40.0f }, { 0.8f, 0.8f, 0.1f, 1.0f });
			//SceneRenderer::DrawQuad({ -1.0f, 3.0f, 20.0f }, { 3.0f, 2.0f }, { 30.0f, 20.0f, 40.0f }, { 0.8f, 0.2f, 0.2f, 1.0f });
			
			// Render Logic above here
			SceneRenderer::EndScene();

			m_Framebuffer->Unbind();
		}
	}

	void Viewport::OnEvent(Event& e)
	{
		m_Camera.OnEvent(e);
	}

}
