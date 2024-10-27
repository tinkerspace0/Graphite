#pragma once

#include "Graphite/Core/Layer.h"

#include "Graphite/Events/ApplicationEvent.h"
#include "Graphite/Events/KeyEvent.h"
#include "Graphite/Events/MouseEvent.h"

namespace Graphite {

	class GRAPHITE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}