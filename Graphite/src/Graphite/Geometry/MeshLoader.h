#pragma once

#include "Mesh.h"
#include "Graphite/Core/Base.h"


namespace Graphite
{
	namespace MeshLoader
	{
		Ref<Mesh> LoadOBJ(const std::string& filepath);
	}
}