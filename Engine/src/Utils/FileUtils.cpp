#include <Utils/FileUtils.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fileUtils
{

bool ReadTextFromFile(std::string_view filepath, std::string& text)
{
	std::ifstream file;
	// ensure ifstream objects can throw exceptions:
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open file
		file.open(filepath);
		std::stringstream shaderStream;
		// read file's buffer contents into streams
		shaderStream << file.rdbuf();
		// close file handler
		file.close();
		// convert stream into string
		text = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return false;
	}

	return true;
}

} // namespace fileUtils