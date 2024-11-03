#include "gfpch.h"
#include "Entity.h"

namespace Graphite {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}