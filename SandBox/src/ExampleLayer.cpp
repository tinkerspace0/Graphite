#include "ExampleLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
	: Layer("ExampleLayer"), m_CameraController(1280.0f / 720.0f, true)
{

	// Square 
	m_SquareVA = Graphite::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Graphite::Ref<Graphite::VertexBuffer> squareVB = Graphite::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ Graphite::ShaderDataType::Float3, "a_Position" },
		{ Graphite::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Graphite::Ref<Graphite::IndexBuffer> squareIB = Graphite::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_Texture = Graphite::Texture2D::Create("assets/textures/Checkerboard.png");
 	m_ChernoLogoTexture = Graphite::Texture2D::Create("assets/textures/ChernoLogo.png");

	// Load shader file and bind the shader
	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	textureShader->Bind();
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Graphite::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Graphite::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Graphite::RenderCommand::Clear();

	Graphite::Renderer::BeginScene(m_CameraController.GetCamera());

	//glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

// 	m_FlatColorShader->Bind();
// 	m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);

	/*for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Graphite::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
		}
	}*/

	 //Triangle
	//Graphite::Renderer::Submit(m_Shader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

	// Get the Texture shader
	auto textureShader = m_ShaderLibrary.Get("Texture");

	// Set uniform values
	textureShader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });	// Set the tint color for texture
	textureShader->SetFloat("u_TilingFactor", 1.0f);		// Set the tiling factor
	textureShader->SetInt("u_Texture", 0);	// Bind the u_Texture to slot 0
	
 	
	// Bind the texture to texture unit 0
	m_ChernoLogoTexture->Bind(0);
	Graphite::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	
	
	m_Texture->Bind(1);
 	Graphite::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	
	Graphite::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Graphite::Event& e)
{
	m_CameraController.OnEvent(e);
}