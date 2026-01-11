#pragma once

#include <Renderer/Mesh.h>
#include <Renderer/Texture.h>

#include <vector>

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;
};

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

	AABB GetLocalAABB() const { return m_axisAlignedBoundingBox; }

private:

	void CalculateAABB(AABB& aaBB);

	std::string m_filepath;
	std::string m_name;
	std::vector<Mesh> m_meshes;
	AABB m_axisAlignedBoundingBox;

	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureMap;

	friend class Core::AssetManager;
};
