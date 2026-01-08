#pragma once

#include <future>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <Renderer/Mesh.h>
#include <Renderer/Texture.h>

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Core
{
	class AssetManager;
}

class Model
{
public:
	Model(const std::string& filepath, std::vector<MeshLoadedData>&& meshData);

	void Draw(Shader& shader) const;

	std::string GetName() const { return m_name; }

private:

	std::string m_filepath;
	std::string m_directory;
	std::string m_name;
	std::vector<Mesh> m_meshes;

	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureMap;

	friend class Core::AssetManager;
};
