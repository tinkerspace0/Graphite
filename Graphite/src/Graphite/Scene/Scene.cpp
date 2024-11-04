#include "gfpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "Graphite/Renderer/VPRenderer.h"

#include <glm/glm.hpp>


namespace Graphite {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcEntity : view)
				{
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

					auto& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

// 	void Scene::OnUpdateRuntime(Timestep ts)
// 	{
// 		// Update scripts
// 		{
// 			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
// 				{
// 					// TODO: Move to Scene::OnScenePlay
// 					if (!nsc.Instance)
// 					{
// 						nsc.Instance = nsc.InstantiateScript();
// 						nsc.Instance->m_Entity = Entity{ entity, this };
// 						nsc.Instance->OnCreate();
// 					}
// 
// 					nsc.Instance->OnUpdate(ts);
// 				});
// 		}
// 
// 		// Render 2D
// 		Camera* mainCamera = nullptr;
// 		glm::mat4 cameraTransform;
// 		{
// 			auto view = m_Registry.view<TransformComponent, CameraComponent>();
// 			for (auto entity : view)
// 			{
// 				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
// 
// 				if (camera.Primary)
// 				{
// 					mainCamera = &camera.Camera;
// 					cameraTransform = transform.GetTransform();
// 					break;
// 				}
// 			}
// 		}
// 
// 	}

// 	void Scene::OnUpdateEditor(Timestep ts, ViewportCamera& camera)
//	{
// 		SceneRenderer::BeginScene(camera);
// 
// 		auto group = m_Registry.group<TransformComponent>(entt::get<>);
// 		for (auto entity : group)
// 		{
// 			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
// 
// 			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
// 		}
// 
// 		Renderer2D::EndScene();
//	}
// 
// 	void Scene::OnViewportResize(uint32_t width, uint32_t height)
// 	{
// 		m_ViewportWidth = width;
// 		m_ViewportHeight = height;
// 
// 		// Resize our non-FixedAspectRatio cameras
// 		auto view = m_Registry.view<CameraComponent>();
// 		for (auto entity : view)
// 		{
// 			auto& cameraComponent = view.get<CameraComponent>(entity);
// 			if (!cameraComponent.FixedAspectRatio)
// 				cameraComponent.Camera.SetViewportSize(width, height);
// 		}
// 
// 	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		// Copy name because we're going to modify component data structure
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		// TODO: Maybe should be assert
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}


}

/*

void Scene::RenderScene(EditorCamera& camera)
{
	Renderer2D::BeginScene(camera);

	// Draw sprites
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}
	}

	// Draw circles
	{
		auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

			Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
		}
	}

	// Draw text
	{
		auto view = m_Registry.view<TransformComponent, TextComponent>();
		for (auto entity : view)
		{
			auto [transform, text] = view.get<TransformComponent, TextComponent>(entity);

			Renderer2D::DrawString(text.TextString, transform.GetTransform(), text, (int)entity);
		}
	}

	Renderer2D::EndScene();
}

*/
