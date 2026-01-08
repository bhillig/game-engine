#include <Renderer/Model.h>

#include <iostream>
#include <filesystem>

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
}

void Model::Draw(Shader& shader) const
{
	for (const auto& mesh : m_meshes)
	{
		mesh.Draw(shader);
	}
}
