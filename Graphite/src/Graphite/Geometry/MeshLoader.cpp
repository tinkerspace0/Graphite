#include "gfpch.h"

#include "MeshLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Graphite {
    namespace MeshLoader {

        struct VertexKey {
            int posIndex;
            int texIndex;
            int normIndex;

            // Equality operator for VertexKey
            bool operator==(const VertexKey& other) const {
                return posIndex == other.posIndex && texIndex == other.texIndex && normIndex == other.normIndex;
            }
        };

        // Custom hash function for VertexKey
        struct VertexKeyHash {
            size_t operator()(const VertexKey& key) const {
                return std::hash<int>()(key.posIndex) ^ (std::hash<int>()(key.texIndex) << 1) ^ (std::hash<int>()(key.normIndex) << 2);
            }
        };

        Ref<Mesh> LoadOBJ(const std::string& filepath) {
            std::ifstream file(filepath);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open .obj file: " + filepath);
            }

            // Temporary storage for parsed data
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> texCoords;
            std::vector<uint32_t> indices;
            std::unordered_map<VertexKey, uint32_t, VertexKeyHash> vertexMap;

            // Final storage for unique mesh vertices
            std::vector<Mesh::Vertex> vertices;

            // Reserve memory to minimize reallocations
            positions.reserve(10000);
            normals.reserve(10000);
            texCoords.reserve(10000);
            vertices.reserve(10000);

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string prefix;
                iss >> prefix;

                if (prefix == "v") { // Vertex position
                    glm::vec3 position;
                    iss >> position.x >> position.y >> position.z;
                    positions.push_back(position);
                }
                else if (prefix == "vn") { // Vertex normal
                    glm::vec3 normal;
                    iss >> normal.x >> normal.y >> normal.z;
                    normals.push_back(normal);
                }
                else if (prefix == "vt") { // Texture coordinate
                    glm::vec2 texCoord;
                    iss >> texCoord.x >> texCoord.y;
                    texCoords.push_back(texCoord);
                }
                else if (prefix == "f") { // Face
                    std::vector<uint32_t> faceIndices;
                    std::string vertexStr;

                    while (iss >> vertexStr) {
                        // Parse vertex index set: v/vt/vn
                        size_t firstSlash = vertexStr.find('/');
                        size_t secondSlash = vertexStr.find('/', firstSlash + 1);

                        int posIndex = std::stoi(vertexStr.substr(0, firstSlash)) - 1;
                        int texIndex = (firstSlash + 1 < secondSlash) ? std::stoi(vertexStr.substr(firstSlash + 1, secondSlash - firstSlash - 1)) - 1 : -1;
                        int normIndex = (secondSlash + 1 < vertexStr.size()) ? std::stoi(vertexStr.substr(secondSlash + 1)) - 1 : -1;

                        VertexKey key = { posIndex, texIndex, normIndex };

                        if (vertexMap.find(key) == vertexMap.end()) {
                            // Create a new vertex
                            Mesh::Vertex vertex;
                            vertex.Position = positions[posIndex];
                            //vertex.TexCoord = (texIndex >= 0 && texIndex < texCoords.size()) ? texCoords[texIndex] : glm::vec2(0.0f);
                            vertex.Normal = (normIndex >= 0 && normIndex < normals.size()) ? normals[normIndex] : glm::vec3(0.0f);
                            vertex.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Default color (white)

                            // Add to vertex list and map
                            vertexMap[key] = static_cast<uint32_t>(vertices.size());
                            vertices.push_back(vertex);
                        }

                        // Add the index for this vertex
                        faceIndices.push_back(vertexMap[key]);
                    }

                    // Triangulate the face
                    for (size_t i = 1; i + 1 < faceIndices.size(); i++) {
                        indices.push_back(faceIndices[0]);
                        indices.push_back(faceIndices[i]);
                        indices.push_back(faceIndices[i + 1]);
                    }
                }
            }

            file.close();

            // Create and return the Mesh object
            Ref<Mesh> mesh = CreateRef<Mesh>(vertices, indices);
            return mesh;
        }

    } // namespace MeshLoader
} // namespace Graphite
