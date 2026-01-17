#pragma once

#include <Renderer/Model.h>
#include <Renderer/ModelLoader.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace Core
{

class AssetManager
{
public:

	static void RequestLoadModel(const std::string& filepath);

	static const std::vector<std::unique_ptr<Model>>& GetModels();

private:

	void RequestLoadModelImpl(const std::string& filepath);

	const std::vector<std::unique_ptr<Model>>& GetModelsImpl() const { return m_models; }

	void Initialize();

	void Update();

	friend class Application;

	static AssetManager& GetAssetManager();
	AssetManager();

	std::vector<std::unique_ptr<Model>> m_models;

	threadsafe_queue<ModelLoadedData> m_modelToConstructQueue;
	ModelLoader m_modelLoader;
};

}
