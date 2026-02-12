#pragma once

#include <Utils/ThreadSafeQueue.h>
#include <Renderer/Model.h>

#include <assimp/scene.h>

namespace Core
{

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
	~ModelLoader();

	void AddModelLoadTaskToQueue(ModelLoadTask&& task);

private:

	bool LoadModel(const std::string& filepath, ModelLoadedData& modelData);

	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshLoadedData>& meshData);

	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<MeshLoadedData>& meshData);

	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, MeshDataTexturePairVec& vec);

	void ProcessTasks_Thread();

	void RequestWorkerThreadStop();

	std::unordered_map<std::string, TextureRef> m_textureCache;
	std::string m_currentModelDirectory;

	ModelLoadedCallbackFunc m_onModelLoadedCallback;
	std::thread m_workerThread;
	threadsafe_queue<ModelLoadTask> m_modelLoadTaskQueue;
	std::atomic<bool> m_stopRequested;

	friend class AssetManager;
};

}