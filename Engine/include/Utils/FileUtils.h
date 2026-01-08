#pragma once

#include <string>

namespace fileUtils
{

	bool ReadTextFromFile(std::string_view filepath, std::string& text);

} // namespace fileUtils