#include <Renderer/Model.h>

#include <iostream>
#include <filesystem>

Model::Model(const std::string& filepath)
	: m_filepath(filepath), m_loaded(false)
{
	std::filesystem::path path(m_filepath);

	// Retrieve the name and directory from the filepath
	m_name = path.filename().stem().string();
	m_directory = m_filepath.substr(0, m_filepath.find_last_of('/'));
}

bool Model::LoadModel()
{
	if (m_loaded)
	{
		return false;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_filepath.c_str(), aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace);

	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cerr << "Failed to import model: " << m_filepath << "\n";
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
		return false;
	}


	processNode(scene->mRootNode, scene);

	m_loaded = true;
	return true;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Process all of a node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.emplace_back(processMesh(mesh, scene));
	}

	// Process all of its children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}
void Model::Draw(Shader& shader)
{
	for (const auto& mesh : m_meshes)
	{
		mesh.Draw(shader);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	if (!mesh || !mesh->HasPositions())
	{
		assert(false && "ERROR: Tried to process invalid mesh!");
		std::cerr << "ERROR: Tried to process invalid mesh: " << mesh->mName.C_Str() << std::endl;
		return Mesh({}, {}, {});
	}

	// Get vertices (positions, normals, and texture coordinates)
	std::vector<Vertex> vertices;
	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasNormals())
		{
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		vertex.textureCoord = mesh->HasTextureCoords(0) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0, 0);
		vertices.emplace_back(vertex);
	}

	// Get indices for each face
	std::vector<unsigned int> indices;
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		const auto& face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}

	// Get textures
	std::vector<TextureRef> textures;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TextureRef> diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

		std::vector<TextureRef> specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
	}

	return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

std::vector<TextureRef> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<TextureRef> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString path;
		mat->GetTexture(type, i, &path);

		const std::string filepath = m_directory + '/' + path.C_Str();

		// Check if the texture has been loaded already
		if (!m_textureCache.contains(filepath))
		{
			// Load the texture
			m_textureCache[filepath] = { typeName, std::make_shared<Texture>(filepath)};
		}

		textures.emplace_back(m_textureCache[filepath]);
	}

	return textures;
}
