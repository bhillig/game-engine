#include <AssetManager.h>
#include <filesystem>
#include <iostream>


namespace Core
{

AssetManager::AssetManager()
	: m_modelLoader([this](ModelLoadedData&& modelData)
	{
		m_modelToConstructQueue.push(std::move(modelData));
	})
{
	m_modelLoaderThread = std::thread(&ModelLoader::ProcessTasks_Thread, &m_modelLoader);
}

AssetManager::~AssetManager()
{
	if (m_modelLoaderThread.joinable())
	{
		m_modelLoaderThread.join();
	}
}

void AssetManager::Update()
{
	// If we have a model ready to be constructed - construct it
	ModelLoadedData modelData;
	if (m_modelToConstructQueue.try_pop(modelData))
	{
		std::cout << "[RESOURCE] Model Loaded: " << modelData.filepath << "\n";
		m_models.emplace_back(std::make_unique<Model>(modelData.filepath, std::move(modelData.meshData)));
	}
}

void AssetManager::RequestLoadModel(const std::string& filepath)
{
	std::filesystem::path path(filepath);
	if (!exists(path))
	{
		return;
	}

	m_modelLoader.AddModelLoadTaskToQueue({ filepath });
}

}
