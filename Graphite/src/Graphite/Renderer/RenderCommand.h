#pragma once

#include "Graphite/Renderer/RendererAPI.h"

namespace Graphite {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			if (!vertexArray || !vertexArray->GetIndexBuffer()) {
				GF_CORE_ERROR("DrawIndexed: Null vertex array or index buffer.");
				return;
			}
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}
		
		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			if (!vertexArray || !vertexArray->GetIndexBuffer()) {
				GF_CORE_ERROR("DrawLines: Null vertex array or index buffer.");
				return;
			}
			s_RendererAPI->DrawLines(vertexArray, count);
		}

		static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}