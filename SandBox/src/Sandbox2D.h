#pragma once

#include "Graphite.h"

class Sandbox2D : public Graphite::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Graphite::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Graphite::Event& e) override;
private:
	Graphite::CameraController m_CameraController;

	// Temp
	Graphite::Ref<Graphite::VertexArray> m_SquareVA;
	Graphite::Ref<Graphite::Shader> m_FlatColorShader;

	Graphite::Ref<Graphite::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};