#pragma once

#include "Graphite/Renderer/ViewportCamera.h"
#include "Graphite/Renderer/Texture.h"
#include "Graphite/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Graphite {

	class SceneRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const ViewportCamera& camera);
		static void EndScene();
		static void FlushLines();

		// Line based Primitives
		static void DrawSquare(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation = { 0.0f, 0.0f, 0.0f }, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
		static void DrawGrid(float spacing = 1.0f, int lineCount = 100, const glm::vec4& color = { 0.4f, 0.3f, 0.2f, 1.0f });
		static void DrawGrid(ViewportCamera& camera, float spacing, int visibleCells, const glm::vec4& color = { 0.4f, 0.3f, 0.2f, 1.0f });

		// Triangle Based Primitives
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation = { 0.0f, 0.0f, 0.0f }, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t LineCount = 0;
			uint32_t TriangleCount = 0;

			uint32_t GetTotalLineVertexCount() const { return LineCount * 2; }
			uint32_t GetTotalLineIndexCount() const { return LineCount * 2; }
		};

		static void ResetStats();
		static Statistics GetStats();

	//private:
		//static void FlushAndReset();
	};
}
