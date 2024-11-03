#pragma once

#include "Viewport.h"
#include "Panels/ObjectInspectorPanel.h"

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

		Ref<Scene> m_ActiveScene;

		std::vector<Ref<Viewport>> m_Viewports;

		ObjectInspectorPanel m_ObjectInspectorPanel;
	};
}