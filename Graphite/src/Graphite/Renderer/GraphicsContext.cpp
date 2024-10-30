#include "gfpch.h"
#include "Graphite/Renderer/GraphicsContext.h"

#include "Graphite/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Graphite {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    GF_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		GF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}