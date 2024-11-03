#pragma once

#include "Graphite/Core/Layer.h"

#include "Graphite/Events/ApplicationEvent.h"
#include "Graphite/Events/KeyEvent.h"
#include "Graphite/Events/MouseEvent.h"


#include "Graphite/Renderer/VertexArray.h"

#include "Viewport.h"


namespace Graphite {


	class ViewportLayer : public Layer {
	public:
		ViewportLayer();
		~ViewportLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

		void CreateViewport(std::string name);

	private:
		bool m_BlockEvents = false;

		std::vector<Ref<Viewport>> m_Viewports;

		//m_PropertiesPanel
		//m_ObjectInspectorPanel


		// ShaderLibrary m_ShaderLibrary;
		//Ref<Shader> m_Shader;
		//Ref<VertexArray> m_VertexArray;

		//Ref<Framebuffer> m_Framebuffer;

		//Ref<Shader> m_FlatColorShader;
		//Ref<VertexArray> m_SquareVA;

		//Ref<Texture2D> m_Texture, m_ImageTexture;

		//OrthographicCameraController m_CameraController;
		//glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
		//glm::vec2 m_ViewportSize;
	};
}