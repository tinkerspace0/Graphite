#pragma once

#include <glm/glm.hpp>


namespace Graphite
{

    class Mesh {
    public:

        struct Vertex {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec4 Color;
            int EntityID = -1;
        };

        // Constructors
        Mesh() = default;
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
            : m_Vertices(vertices), m_Indices(indices) {}
        ~Mesh() {}

        // Accessors
        inline const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
        inline const std::vector<uint32_t>& GetIndices() const { return m_Indices; }

        // Utility methods
        inline size_t GetVertexCount() const { return m_Vertices.size(); }
        inline size_t GetIndexCount() const { return m_Indices.size(); }

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
    };
}
