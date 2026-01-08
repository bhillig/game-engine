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
	AssetManager();
	~AssetManager();

	void Update();

	void RequestLoadModel(const std::string& filepath);

	const std::vector<std::unique_ptr<Model>>& GetModels() const { return m_models; }

private:

	std::thread m_modelLoaderThread;
	threadsafe_queue<ModelLoadedData> m_modelToConstructQueue;
	ModelLoader m_modelLoader;

	std::vector<std::unique_ptr<Model>> m_models; // Maps filepath to model
};

}
