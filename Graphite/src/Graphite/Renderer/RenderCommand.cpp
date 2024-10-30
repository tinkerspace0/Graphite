#include "gfpch.h"
#include "Graphite/Renderer/RenderCommand.h"

namespace Graphite {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}