#pragma once

#include "Graphite/Core/Timestep.h"
#include "Graphite/Renderer/ViewportCamera.h"

#include "entt.hpp"

namespace Graphite {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		//Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		//void OnUpdateEditor(Timestep ts, ViewportCamera& camera);
		
		//Entity DuplicateEntity(Entity entity);

		//Entity FindEntityByName(std::string_view name);
		//Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class ObjectInspectorPanel;
	};

}