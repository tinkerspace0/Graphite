#pragma once

#include "Graphite/Core/Layer.h"

#include "Graphite/Events/ApplicationEvent.h"
#include "Graphite/Events/KeyEvent.h"
#include "Graphite/Events/MouseEvent.h"

namespace Graphite {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = false;
		float m_Time = 0.0f;
	};

}