#pragma once

#include "Graphite/Core/Timestep.h"
#include "Graphite/Renderer/ViewportCamera.h"
#include "Graphite/Core/UUID.h"
#include "entt.hpp"

namespace Graphite {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		//void OnUpdateEditor(Timestep ts, ViewportCamera& camera);
		
		//Entity DuplicateEntity(Entity entity);

		//Entity FindEntityByName(std::string_view name);
		//Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		Entity DuplicateEntity(Entity entity);
		
		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class ObjectInspectorPanel;
	};

}