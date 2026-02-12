#include <Renderer/Model.h>

#include <iostream>
#include <filesystem>

namespace Core
{

Model::Model(const std::string& filepath, std::vector<MeshLoadedData>&& meshData)
	: m_filepath(filepath)
{
	std::filesystem::path path(m_filepath);

	// Retrieve the name from the filepath
	m_name = path.filename().stem().string();

	// Build the texture map
	for (auto& data : meshData)
	{
		for (auto& [textureTypeName, textureFilePath] : data.textureData)
		{
			if (!m_textureMap.contains(textureFilePath))
			{
				m_textureMap[textureFilePath] = std::make_shared<Texture>(textureFilePath);
			}
		}
	}

	for (MeshLoadedData& data : meshData)
	{
		std::vector<TextureRef> textureReferences;
		for (auto& [textureTypeName, textureFilePath] : data.textureData)
		{
			textureReferences.emplace_back(textureTypeName, m_textureMap[textureFilePath]);
		}

		MeshData meshData{
		std::move(data.vertices),
		std::move(data.indices),
		std::move(textureReferences)
		};

		m_meshes.emplace_back(std::move(meshData));
	}

	// Calculate model's local Axis Aligned Bounding Box
	CalculateAABB(m_axisAlignedBoundingBox);
}

void Model::Draw(Shader& shader) const
{
	for (const auto& mesh : m_meshes)
	{
		mesh.Draw(shader);
	}
}

void Model::CalculateAABB(AABB& aaBB)
{
	aaBB.min = glm::vec3(std::numeric_limits<float>::max());
	aaBB.max = glm::vec3(std::numeric_limits<float>::min());

	for (const auto& mesh : m_meshes)
	{
		for (const auto& vertex : mesh.GetVertices())
		{
			aaBB.min.x = std::min(aaBB.min.x, vertex.position.x);
			aaBB.min.y = std::min(aaBB.min.y, vertex.position.y);
			aaBB.min.z = std::min(aaBB.min.z, vertex.position.z);

			aaBB.max.x = std::max(aaBB.max.x, vertex.position.x);
			aaBB.max.y = std::max(aaBB.max.y, vertex.position.y);
			aaBB.max.z = std::max(aaBB.max.z, vertex.position.z);
		}
	}
}

}
