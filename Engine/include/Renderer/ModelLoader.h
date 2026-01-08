#pragma once

#include <Utils/ThreadSafeQueue.h>
#include <Renderer/Model.h>

struct ModelLoadTask
{
	std::string filepath;
};

struct ModelLoadedData
{
	std::string filepath;
	std::vector<MeshLoadedData> meshData;
};

using ModelLoadedCallbackFunc = std::function<void(ModelLoadedData&&)>;

class ModelLoader
{
public:
	ModelLoader(ModelLoadedCallbackFunc callback);

	void AddModelLoadTaskToQueue(ModelLoadTask&& task);

private:

	bool LoadModel(const std::string& filepath, ModelLoadedData& modelData);

	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshLoadedData>& meshData);

	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<MeshLoadedData>& meshData);

	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, MeshDataTexturePairVec& vec);

	void ProcessTasks_Thread();

	std::unordered_map<std::string, TextureRef> m_textureCache;
	std::string m_currentModelDirectory;
	ModelLoadedCallbackFunc m_onModelLoadedCallback;
	threadsafe_queue<ModelLoadTask> m_modelLoadTaskQueue;

	friend class Core::AssetManager;
};

