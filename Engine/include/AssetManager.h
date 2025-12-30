#pragma once

#include <Renderer/Model.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace Core
{

class AssetManager
{
public:
	AssetManager() = default;

	bool ImportModel(const std::string& filepath);

	const std::unordered_map<std::string, std::unique_ptr<Model>>& GetModels() const { return m_models; }

private:
	std::unordered_map<std::string, std::unique_ptr<Model>> m_models; // Maps filepath to model

};

}
