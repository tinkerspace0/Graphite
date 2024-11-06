#include "gfpch.h"
#include "Graphite/Scene/SceneRenderer.h"
#include "Graphite/Renderer/VertexArray.h"
#include "Graphite/Renderer/Shader.h"
#include "Graphite/Renderer/UniformBuffer.h"
#include "Graphite/Renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Graphite {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Scene-only
		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Scene-only
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Scene-only
		int EntityID;
	};

	struct SceneRendererData
	{
		static const uint32_t MaxPrimitives = 20000;
		static const uint32_t MaxVertices = MaxPrimitives * 4;
		static const uint32_t MaxIndices = MaxPrimitives * 6;

		// Quad Data
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Circle Data
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;
		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		//Line Data
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;
		uint32_t LineIndexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		glm::vec4 QuadVertexPositions[4];

		SceneRenderer::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
 		Ref<UniformBuffer> CameraUniformBuffer;

	};

	static SceneRendererData s_Data;

	void SceneRenderer::Init()
	{
		GF_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Int,    "a_EntityID"     }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Circles
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		// Lines
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
			});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];
		
		uint32_t* lineIndices = new uint32_t[s_Data.MaxIndices];
		for (uint32_t i = 0; i < s_Data.MaxIndices; i++)
			lineIndices[i] = i;  // Simple sequential indexing for line rendering

		Ref<IndexBuffer> lineIB = IndexBuffer::Create(lineIndices, s_Data.MaxIndices);
		s_Data.LineVertexArray->SetIndexBuffer(lineIB);
		delete[] lineIndices;


		s_Data.QuadShader = Shader::Create("assets/shaders/Scene_QuadShader.glsl");
		s_Data.CircleShader = Shader::Create("assets/shaders/Scene_CircleShader.glsl");
		s_Data.LineShader = Shader::Create("assets/shaders/Scene_LineShader.glsl");

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(SceneRendererData::CameraData), 0);
	}

	void SceneRenderer::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
		delete[] s_Data.CircleVertexBufferBase;
		delete[] s_Data.LineVertexBufferBase;
		
	}

	void SceneRenderer::BeginScene(const ViewportCamera& camera)
	{

		GF_PROFILE_FUNCTION();
		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(SceneRendererData::CameraData));
		StartBatch();
	}

	void SceneRenderer::EndScene()
	{
		Flush();
	}

	void SceneRenderer::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineIndexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
	}

	void SceneRenderer::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineIndexCount);
			s_Data.Stats.DrawCalls++;
		}

	}

	void SceneRenderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void SceneRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID)
	{
		s_Data.LineVertexBufferPtr->Position = start;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = end;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineIndexCount += 2;

		s_Data.Stats.PrimitivesCount++;
	}

	void SceneRenderer::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void SceneRenderer::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	void SceneRenderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation, const glm::vec4& color, int entityID ) 
	{
		GF_PROFILE_FUNCTION();

		// Create a rotation matrix from the rotation vector (in radians)
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f });

		// Create the transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			rotationMatrix *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		// Draw the quad with the specified transform, color, and entity ID
		DrawQuad(transform, color, entityID);
	}
	
	void SceneRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID ) 
	{
		GF_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= SceneRendererData::MaxIndices)
			NextBatch();


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.PrimitivesCount++;
	}

	void SceneRenderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		GF_PROFILE_FUNCTION();

		// Check if we need to start a new batch
		if (s_Data.CircleIndexCount >= SceneRendererData::MaxIndices)
			NextBatch();

		// Set up circle vertices
		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		// Increment the index count for the circle
		s_Data.CircleIndexCount += 6;

		// Update statistics
		s_Data.Stats.PrimitivesCount++;
	}

	void SceneRenderer::DrawGrid(float spacing, int lineCount, const glm::vec4& color)
	{
		float GridSize = spacing * lineCount;

		for (int i = -lineCount; i <= lineCount; i++)
		{
			// Horizontal lines
			DrawLine(glm::vec3(-GridSize, 0.0f, i * spacing), glm::vec3(GridSize, 0.0f, i * spacing), color);
			// Vertical lines
			DrawLine(glm::vec3(i * spacing, 0.0f, -GridSize), glm::vec3(i * spacing, 0.0f, GridSize), color);
		}
	}

	void SceneRenderer::DrawGrid(ViewportCamera& camera, float spacing, int visibleCells, const glm::vec4& color)
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

	float SceneRenderer::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void SceneRenderer::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	void SceneRenderer::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	SceneRenderer::Statistics SceneRenderer::GetStats()
	{
		return s_Data.Stats;
	}
}
