#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string>

#include <memory>

#include <Renderer/VertexArray.h>
#include <Renderer/Shader.h>
#include <Renderer/Texture.h>

namespace Core
{

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoord;
};

struct TextureRef
{
	std::string type;
	std::shared_ptr<Texture> texture;
};

// Pairs a texture type (e.g "texture_diffuse" with a filepath)
using MeshDataTexturePairVec = std::vector<std::pair<std::string, std::string>>;

struct MeshLoadedData
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	MeshDataTexturePairVec textureData; // Pairs a texture type (e.g "texture_diffuse" with a filepath)
};

using MeshDataTextureDataPairVec = std::vector<std::pair<std::string, std::shared_ptr<Texture>>>;


struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureRef> textureReferences; // Pairs a texture type (e.g "texture_diffuse" with a texture)
};

class Mesh
{
public:
	Mesh(MeshData&& meshData);
	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	const std::vector<Vertex>& GetVertices() const { return m_vertices; }

	const std::vector<unsigned int>& GetIndices() const { return m_indices; }

	const std::vector<TextureRef>& GetTextures() const { return m_textures; }

	void Draw(Shader& shader) const;

private:

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<TextureRef> m_textures;

	std::shared_ptr<VertexArray> m_vao;
};

}