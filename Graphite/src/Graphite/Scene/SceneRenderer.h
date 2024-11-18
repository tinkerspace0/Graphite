#pragma once

#include "Graphite/Renderer/ViewportCamera.h"
#include "Graphite/Renderer/OrthographicCamera.h"
#include "Graphite/Renderer/Texture.h"
#include "Graphite/Renderer/Shader.h"
#include "Graphite/Geometry/Mesh.h"
#include <glm/glm.hpp>

namespace Graphite {

	class SceneRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const ViewportCamera& camera);
		static void EndScene();
		static void Flush();

		//Primitives
		// GL_LINE Call based Primitives
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation, const glm::vec4& color, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);


		// GL_TRIANGLE Call based Primitives
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);


		// Grid Rendering for viewport
		static void DrawGrid(ViewportCamera& camera, float spacing, int visibleCells, const glm::vec4& color = { 0.2f, 0.2f, 0.2f, 1.0f });
		static void DrawGrid(float spacing = 1.0f, int lineCount = 100, const glm::vec4& color = { 0.2f, 0.2f, 0.2f, 1.0f });


		// Mesh Rendering for Scene
		static void DrawMesh(Ref<Mesh> mesh, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation, int entityID = -1);
		static void DrawMesh(Ref<Mesh> mesh, const glm::mat4& transform, int entityID=-1);

		static float GetLineWidth();
		static void SetLineWidth(float width);


		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t PrimitivesCount = 0;

			uint32_t GetTotalVertexCount() const { return PrimitivesCount * 4; }
			uint32_t GetTotalIndexCount() const { return PrimitivesCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();


	private:
		static void StartBatch();
		static void NextBatch();
	};
}
