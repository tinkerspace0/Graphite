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
		static const uint32_t MaxVertices = 10000;
		static const uint32_t MaxIndices = MaxVertices * 2;

		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;
		Ref<Shader> PrimShader;
		glm::vec4 LineColor;

		VertexData* VertexBufferBase = nullptr;
		VertexData* VertexBufferPtr = nullptr;
		uint32_t IndexCount = 0;

		ViewportRenderer::Statistics Stats;
	};

	static ViewportRendererData s_Data;

	void ViewportRenderer::Init()
	{
		s_Data.VertexArray = VertexArray::Create();

		// Create Vertex Buffer
		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(VertexData));
		s_Data.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
			});
		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);

		s_Data.VertexBufferBase = new VertexData[s_Data.MaxVertices];

		// Create and configure Index Buffer
		uint32_t indices[s_Data.MaxIndices];
		for (uint32_t i = 0; i < s_Data.MaxIndices; i++)
			indices[i] = i;  // Simple sequential indexing for line rendering

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_Data.MaxIndices);
		s_Data.VertexArray->SetIndexBuffer(indexBuffer);

		// Load a basic shader for color rendering
		s_Data.PrimShader = Shader::Create("assets/shaders/Basic.glsl");
		s_Data.PrimShader->Bind();
	}

	void ViewportRenderer::Shutdown()
	{
		delete[] s_Data.VertexBufferBase;
	}

	void ViewportRenderer::BeginScene(const ViewportCamera& camera)
	{
		s_Data.PrimShader->Bind();
		s_Data.PrimShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
		s_Data.IndexCount = 0;
	}

	void ViewportRenderer::EndScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase);
		s_Data.VertexBuffer->SetData(s_Data.VertexBufferBase, dataSize);
		Flush();
	}

	void ViewportRenderer::Flush()
	{
		if (s_Data.IndexCount == 0)
			return;

		RenderCommand::DrawIndexedLines(s_Data.VertexArray, s_Data.IndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void ViewportRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
	{
		s_Data.VertexBufferPtr->Position = start;
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr++;

		s_Data.VertexBufferPtr->Position = end;
		s_Data.VertexBufferPtr->Color = color;
		s_Data.VertexBufferPtr++;

		s_Data.IndexCount += 2;
		s_Data.Stats.LineCount++;
	}

	void ViewportRenderer::DrawSquare(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec3& rotation)
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
