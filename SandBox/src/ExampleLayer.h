#pragma once

#include "Graphite.h"

class ExampleLayer : public Graphite::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Graphite::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Graphite::Event& e) override;
private:
	Graphite::ShaderLibrary m_ShaderLibrary;
	Graphite::Ref<Graphite::Shader> m_Shader;
	Graphite::Ref<Graphite::VertexArray> m_VertexArray;

	Graphite::Ref<Graphite::Shader> m_FlatColorShader;
	Graphite::Ref<Graphite::VertexArray> m_SquareVA;

	Graphite::Ref<Graphite::Texture2D> m_Texture, m_ChernoLogoTexture;

	Graphite::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};
