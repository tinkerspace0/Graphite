#include "gfpch.h"
#include "Graphite/Viewport/Renderer/ViewportRenderer.h"
#include "Graphite/Renderer/VertexArray.h"
#include "Graphite/Renderer/Shader.h"
#include "Graphite/Renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Graphite {

	struct VertexData
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct ViewportRendererData
	{
		static const uint32_t MaxLineVertices = 10000;
		static const uint32_t MaxLineIndices = MaxLineVertices * 2;

		static const uint32_t MaxTriangleVertices = 10000;
		static const uint32_t MaxTriangleIndices = MaxTriangleVertices * 6;

		// Line data
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		VertexData* LineVertexBufferBase = nullptr;
		VertexData* LineVertexBufferPtr = nullptr;
		uint32_t LineIndexCount = 0;
		glm::vec4 LineColor;
		
		
		// Triangle Data
		Ref<VertexArray> TriangleVertexArray;
		Ref<VertexBuffer> TriangleVertexBuffer;
		VertexData* TriangleLineVertexBufferBase = nullptr;
		VertexData* TriangleLineVertexBufferPtr = nullptr;
		uint32_t TriangleIndexCount = 0;
		glm::vec4 TriangleColor;

		// Shader
		Ref<Shader> BasicShader;

		ViewportRenderer::Statistics Stats;
	};

	static ViewportRendererData s_Data;

	void ViewportRenderer::Init()
	{
		s_Data.LineVertexArray = VertexArray::Create();

		// Create Vertex Buffer
		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxLineVertices * sizeof(VertexData));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
			});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);

		s_Data.LineVertexBufferBase = new VertexData[s_Data.MaxLineVertices];

		// Create and configure Index Buffer
		uint32_t indices[s_Data.MaxLineIndices];
		for (uint32_t i = 0; i < s_Data.MaxLineIndices; i++)
			indices[i] = i;  // Simple sequential indexing for line rendering

		Ref<IndexBuffer> indexLineBuffer = IndexBuffer::Create(indices, s_Data.MaxLineIndices);
		s_Data.LineVertexArray->SetIndexBuffer(indexLineBuffer);

		// Load a basic shader for color rendering
		s_Data.BasicShader = Shader::Create("assets/shaders/Basic.glsl");
		s_Data.BasicShader->Bind();
	}

	void ViewportRenderer::Shutdown()
	{
		delete[] s_Data.LineVertexBufferBase;
		delete[] s_Data.TriangleLineVertexBufferBase;
	}

	void ViewportRenderer::BeginScene(const ViewportCamera& camera)
	{
		s_Data.BasicShader->Bind();
		s_Data.BasicShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
		s_Data.LineIndexCount = 0;
	}

	void ViewportRenderer::EndScene()
	{
		FlushLines();
	}

	void ViewportRenderer::FlushLines()
	{
		if (s_Data.LineIndexCount == 0) return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);
		RenderCommand::DrawIndexedLines(s_Data.LineVertexArray, s_Data.LineIndexCount);
		s_Data.Stats.DrawCalls++;
		s_Data.LineIndexCount = 0;
	}

	void ViewportRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
	{
		s_Data.LineVertexBufferPtr->Position = start;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = end;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineIndexCount += 2;
		s_Data.Stats.LineCount++;
	}

	void ViewportRenderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation, const glm::vec4& color)
	{
		// Calculate half size
		glm::vec3 halfSize = glm::vec3(size * 0.5f, 0.0f);

		// Define vertices for an unrotated square centered at the origin
		glm::vec3 vertices[4] = {
			glm::vec3(-halfSize.x, 0.0f, -halfSize.y),
			glm::vec3(halfSize.x, 0.0f, -halfSize.y),
			glm::vec3(halfSize.x, 0.0f, halfSize.y),
			glm::vec3(-halfSize.x, 0.0f, halfSize.y)
		};

		// Create rotation matrix around the Y-axis
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

		// Apply rotation and translation to each vertex
		for (int i = 0; i < 4; ++i)
		{
			vertices[i] = glm::vec3(rotationMatrix * glm::vec4(vertices[i], 1.0f)) + position;
		}

		// Draw lines between rotated vertices
		DrawLine(vertices[0], vertices[1], color);
		DrawLine(vertices[1], vertices[2], color);
		DrawLine(vertices[2], vertices[3], color);
		DrawLine(vertices[3], vertices[0], color);
	}

	void ViewportRenderer::DrawGrid(float spacing, int lineCount, const glm::vec4& color)
	{
		float GridSize = spacing * lineCount / 1.0f;

		for (int i = -lineCount; i <= lineCount; i++)
		{
			// Horizontal lines
			DrawLine(glm::vec3(-GridSize, 0.0f, i * spacing), glm::vec3(GridSize, 0.0f, i * spacing), color);
			// Vertical lines
			DrawLine(glm::vec3(i * spacing, 0.0f, -GridSize), glm::vec3(i * spacing, 0.0f, GridSize), color);
		}
	}


	void ViewportRenderer::DrawGrid(ViewportCamera& camera, float spacing, int visibleCells, const glm::vec4& color)
	{
		glm::vec3 cameraPosition = camera.GetPosition();

		// Round the camera position to the nearest grid interval
		float startX = std::floor(cameraPosition.x / spacing) * spacing;
		float startY = std::floor(cameraPosition.y / spacing) * spacing;

		int halfCellCount = visibleCells / 2;

		// Calculate grid boundaries centered around the camera's aligned position
		for (int i = -halfCellCount; i <= halfCellCount; ++i) {
			float offset = i * spacing;

			// Vertical lines (along Z-axis)
			DrawLine(glm::vec3(startX + offset, 0.0f, startY - halfCellCount * spacing),
				glm::vec3(startX + offset, 0.0f, startY + halfCellCount * spacing), color);

			// Horizontal lines (along X-axis)
			DrawLine(glm::vec3(startX - halfCellCount * spacing, 0.0f, startY + offset),
				glm::vec3(startX + halfCellCount * spacing, 0.0f, startY + offset), color);
		}
	}

	void ViewportRenderer::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	ViewportRenderer::Statistics ViewportRenderer::GetStats()
	{
		return s_Data.Stats;
	}
}
