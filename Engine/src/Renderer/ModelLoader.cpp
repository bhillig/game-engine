#include <Renderer/ModelLoader.h>

#include <iostream>

ModelLoader::ModelLoader(ModelLoadedCallbackFunc callback)
	: m_onModelLoadedCallback(std::move(callback))
{
}

void ModelLoader::AddModelLoadTaskToQueue(ModelLoadTask&& task)
{
	m_modelLoadTaskQueue.push(std::move(task));
}

bool ModelLoader::LoadModel(const std::string& filepath, ModelLoadedData& modelData)
{

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace);

	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cerr << "Failed to import model: " << filepath << "\n";
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
		return false;
	}

	// Set filepath
	modelData.filepath = filepath;

	// TODO: Improve this - ModelLoader should not need to know the current model's directory like this
	m_currentModelDirectory = filepath.substr(0, filepath.find_last_of('/'));

	// Set model into mesh data
	ProcessNode(scene->mRootNode, scene, modelData.meshData);

	return true;
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshLoadedData>& meshData)
{
	// Process all of a node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, meshData);
	}

	// Process all of its children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, meshData);
	}
}

void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<MeshLoadedData>& meshData)
{
	if (!mesh || !mesh->HasPositions())
	{
		assert(false && "ERROR: Tried to process invalid mesh!");
		std::cerr << "ERROR: Tried to process invalid mesh: " << mesh->mName.C_Str() << "\n";
		return;
	}

	// Get vertices (positions, normals, and texture coordinates)
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
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

	MeshDataTexturePairVec textureData;

	// Get texture data
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textureData);
		LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textureData);
	}

	meshData.emplace_back(std::move(vertices), std::move(indices), std::move(textureData));
}

void ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, MeshDataTexturePairVec& vec)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString path;
		mat->GetTexture(type, i, &path);

		const std::string filepath = m_currentModelDirectory + '/' + path.C_Str();
		vec.emplace_back(typeName, filepath);
	}
}

void ModelLoader::ProcessTasks_Thread()
{
	while (true)
	{
		ModelLoadTask task;
		m_modelLoadTaskQueue.wait_and_pop(task);

		// Load the model and notify callback function
		ModelLoadedData data;
		if (LoadModel(task.filepath, data))
		{
			m_onModelLoadedCallback(std::move(data));
		}
	}
}
