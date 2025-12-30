#pragma once

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
	Model(const std::string& filepath);

	void Draw(Shader& shader);

	std::string GetName() const { return m_name; }

private:

	bool LoadModel();

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureRef> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

	std::vector<Mesh> m_meshes;
	std::string m_directory;
	std::string m_name;
	std::string m_filepath;
	bool m_loaded;

	std::unordered_map<std::string, TextureRef> m_textureCache;

	friend class Core::AssetManager;
};
