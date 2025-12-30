#include <AssetManager.h>
#include <filesystem>

namespace Core
{

bool AssetManager::ImportModel(const std::string& filepath)
{
	std::filesystem::path path(filepath);
	if (!exists(path))
	{
		return false;
	}

	if (m_models.contains(filepath))
	{
		return false;
	}

	std::unique_ptr<Model> model = std::make_unique<Model>(path.string());
	if (!model->LoadModel())
	{
		return false;
	}

	m_models[filepath] = std::move(model);
	return true;
}

}
