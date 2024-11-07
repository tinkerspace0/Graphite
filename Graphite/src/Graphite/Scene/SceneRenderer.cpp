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
		static const uint32_t MaxPrimitives = 1000;
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

		float LineWidth = 1.0f;

		glm::vec4 QuadVertexPositions[4];

		SceneRenderer::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
 		Ref<UniformBuffer> CameraUniformBuffer;

	};

	static SceneRendererData s_SRData;

	void SceneRenderer::Init()
	{
		GF_PROFILE_FUNCTION();

		s_SRData.QuadVertexArray = VertexArray::Create();

		s_SRData.QuadVertexBuffer = VertexBuffer::Create(s_SRData.MaxVertices * sizeof(QuadVertex));
		s_SRData.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Int,    "a_EntityID"     }
			});
		s_SRData.QuadVertexArray->AddVertexBuffer(s_SRData.QuadVertexBuffer);

		s_SRData.QuadVertexBufferBase = new QuadVertex[s_SRData.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_SRData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_SRData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_SRData.MaxIndices);
		s_SRData.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_SRData.QuadVertexPositions[0] = { -0.5f,  10.1f, -0.5f, 1.0f };
		s_SRData.QuadVertexPositions[1] = {  0.5f,  20.2f, -0.5f, 1.0f };
		s_SRData.QuadVertexPositions[2] = {  0.5f,  30.3f,  0.5f, 1.0f };
		s_SRData.QuadVertexPositions[3] = { -0.5f,  40.4f,  0.5f, 1.0f };

		// Circles
		s_SRData.CircleVertexArray = VertexArray::Create();

		s_SRData.CircleVertexBuffer = VertexBuffer::Create(s_SRData.MaxVertices * sizeof(CircleVertex));
		s_SRData.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});
		s_SRData.CircleVertexArray->AddVertexBuffer(s_SRData.CircleVertexBuffer);
		s_SRData.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_SRData.CircleVertexBufferBase = new CircleVertex[s_SRData.MaxVertices];

		// Lines
		s_SRData.LineVertexArray = VertexArray::Create();

		s_SRData.LineVertexBuffer = VertexBuffer::Create(s_SRData.MaxVertices * sizeof(LineVertex));
		s_SRData.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
			});
		s_SRData.LineVertexArray->AddVertexBuffer(s_SRData.LineVertexBuffer);
		s_SRData.LineVertexBufferBase = new LineVertex[s_SRData.MaxVertices];
		
		uint32_t* lineIndices = new uint32_t[s_SRData.MaxIndices];
		for (uint32_t i = 0; i < s_SRData.MaxIndices; i++)
			lineIndices[i] = i;  // Simple sequential indexing for line rendering

		Ref<IndexBuffer> lineIB = IndexBuffer::Create(lineIndices, s_SRData.MaxIndices);
		s_SRData.LineVertexArray->SetIndexBuffer(lineIB);
		delete[] lineIndices;


		s_SRData.QuadShader = Shader::Create("assets/shaders/Scene_QuadShader.glsl");
		s_SRData.CircleShader = Shader::Create("assets/shaders/Scene_CircleShader.glsl");
		s_SRData.LineShader = Shader::Create("assets/shaders/Scene_LineShader.glsl");

		s_SRData.CameraUniformBuffer = UniformBuffer::Create(sizeof(SceneRendererData::CameraData), 0);
	}

	void SceneRenderer::Shutdown()
	{
		delete[] s_SRData.QuadVertexBufferBase;
		delete[] s_SRData.CircleVertexBufferBase;
		delete[] s_SRData.LineVertexBufferBase;
		
	}

	void SceneRenderer::BeginScene(const ViewportCamera& camera)
	{

		GF_PROFILE_FUNCTION();
		s_SRData.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_SRData.CameraUniformBuffer->SetData(&s_SRData.CameraBuffer, sizeof(SceneRendererData::CameraData));
		StartBatch();
	}

	void SceneRenderer::EndScene()
	{
		Flush();
	}

	void SceneRenderer::StartBatch()
	{
		s_SRData.QuadIndexCount = 0;
		s_SRData.QuadVertexBufferPtr = s_SRData.QuadVertexBufferBase;

		s_SRData.CircleIndexCount = 0;
		s_SRData.CircleVertexBufferPtr = s_SRData.CircleVertexBufferBase;

		s_SRData.LineIndexCount = 0;
		s_SRData.LineVertexBufferPtr = s_SRData.LineVertexBufferBase;
	}

	void SceneRenderer::Flush()
	{
		if (s_SRData.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_SRData.QuadVertexBufferPtr - (uint8_t*)s_SRData.QuadVertexBufferBase);
			s_SRData.QuadVertexBuffer->SetData(s_SRData.QuadVertexBufferBase, dataSize);

			s_SRData.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_SRData.QuadVertexArray, s_SRData.QuadIndexCount);
			s_SRData.Stats.DrawCalls++;
		}

		if (s_SRData.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_SRData.CircleVertexBufferPtr - (uint8_t*)s_SRData.CircleVertexBufferBase);
			s_SRData.CircleVertexBuffer->SetData(s_SRData.CircleVertexBufferBase, dataSize);

			s_SRData.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_SRData.CircleVertexArray, s_SRData.CircleIndexCount);
			s_SRData.Stats.DrawCalls++;
		}

		if (s_SRData.LineIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_SRData.LineVertexBufferPtr - (uint8_t*)s_SRData.LineVertexBufferBase);
			s_SRData.LineVertexBuffer->SetData(s_SRData.LineVertexBufferBase, dataSize);

			s_SRData.LineShader->Bind();
			RenderCommand::SetLineWidth(s_SRData.LineWidth);
			RenderCommand::DrawLines(s_SRData.LineVertexArray, s_SRData.LineIndexCount);
			s_SRData.Stats.DrawCalls++;
		}

	}

	void SceneRenderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void SceneRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID)
	{
		s_SRData.LineVertexBufferPtr->Position = start;
		s_SRData.LineVertexBufferPtr->Color = color;
		s_SRData.LineVertexBufferPtr->EntityID = entityID;
		s_SRData.LineVertexBufferPtr++;

		s_SRData.LineVertexBufferPtr->Position = end;
		s_SRData.LineVertexBufferPtr->Color = color;
		s_SRData.LineVertexBufferPtr->EntityID = entityID;
		s_SRData.LineVertexBufferPtr++;

		s_SRData.LineIndexCount += 2;

		s_SRData.Stats.PrimitivesCount++;
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
			lineVertices[i] = transform * s_SRData.QuadVertexPositions[i];

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
			glm::scale(glm::mat4(1.0f), { size.x, 1.0f, size.y });

		// Draw the quad with the specified transform, color, and entity ID
		DrawQuad(transform, color);
	}
	
	void SceneRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID ) 
	{
		GF_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_SRData.QuadIndexCount >= SceneRendererData::MaxIndices)
			NextBatch();


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_SRData.QuadVertexBufferPtr->Position = glm::vec3(transform * s_SRData.QuadVertexPositions[i]);
			s_SRData.QuadVertexBufferPtr->Color = color;
			s_SRData.QuadVertexBufferPtr->EntityID = entityID;
			s_SRData.QuadVertexBufferPtr++;
		}

		s_SRData.QuadIndexCount += 6;

		s_SRData.Stats.PrimitivesCount++;
	}

	void SceneRenderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		GF_PROFILE_FUNCTION();

		// Check if we need to start a new batch
		if (s_SRData.CircleIndexCount >= SceneRendererData::MaxIndices)
			NextBatch();

		// Set up circle vertices
		for (size_t i = 0; i < 4; i++)
		{
			s_SRData.CircleVertexBufferPtr->WorldPosition = transform * s_SRData.QuadVertexPositions[i];
			s_SRData.CircleVertexBufferPtr->LocalPosition = s_SRData.QuadVertexPositions[i] * 2.0f;
			s_SRData.CircleVertexBufferPtr->Color = color;
			s_SRData.CircleVertexBufferPtr->Thickness = thickness;
			s_SRData.CircleVertexBufferPtr->Fade = fade;
			s_SRData.CircleVertexBufferPtr->EntityID = entityID;
			s_SRData.CircleVertexBufferPtr++;
		}

		// Increment the index count for the circle
		s_SRData.CircleIndexCount += 6;

		// Update statistics
		s_SRData.Stats.PrimitivesCount++;
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
		return s_SRData.LineWidth;
	}

	void SceneRenderer::SetLineWidth(float width)
	{
		s_SRData.LineWidth = width;
	}

	void SceneRenderer::ResetStats()
	{
		memset(&s_SRData.Stats, 0, sizeof(Statistics));
	}

	SceneRenderer::Statistics SceneRenderer::GetStats()
	{
		return s_SRData.Stats;
	}
}
