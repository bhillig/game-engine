#include <AssetManager.h>

#include <Application.h>

#include <filesystem>

namespace Core
{

AssetManager::AssetManager()
	: m_modelLoader([this](ModelLoadedData&& modelData)
	{
		m_modelToConstructQueue.push(std::move(modelData));
	})
{
}

bool AssetManager::Initialize()
{
	return true;
}

void AssetManager::Update()
{
	// If we have a model ready to be constructed - construct it
	ModelLoadedData modelData;
	if (m_modelToConstructQueue.try_pop(modelData))
	{
		LOG_CORE_INFO("Model Loaded: {}", modelData.filepath)
		m_models.emplace_back(std::make_unique<Model>(modelData.filepath, std::move(modelData.meshData)));
	}
}

void AssetManager::RequestLoadModel(const std::string& filepath)
{
	GetAssetManager().RequestLoadModelImpl(filepath);
}

const std::vector<std::unique_ptr<Model>>& AssetManager::GetModels()
{
	return GetAssetManager().GetModelsImpl();
}

void AssetManager::RequestLoadModelImpl(const std::string& filepath)
{
	std::filesystem::path path(filepath);
	if (!exists(path))
	{
		return;
	}

	m_modelLoader.AddModelLoadTaskToQueue({ filepath });
}

AssetManager& AssetManager::GetAssetManager()
{
	return Application::GetApp()->GetAssetManager();
}

}
