#pragma once
#pragma once

#include "Graphite/Core/Base.h"
#include "Graphite/Scene/Scene.h"
#include "Graphite/Scene/Entity.h"

namespace Graphite {

	class ObjectInspectorPanel
	{
	public:
		ObjectInspectorPanel() = default;
		ObjectInspectorPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}
