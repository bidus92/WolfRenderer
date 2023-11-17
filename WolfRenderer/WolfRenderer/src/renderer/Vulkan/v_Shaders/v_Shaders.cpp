#include "pch.h"
#include "v_Shaders.h"
#include <fstream>

namespace WolfRenderer
{
	v_Shaders::v_Shaders()
	{

	}

	v_Shaders::~v_Shaders()
	{

	}

	inline std::vector<char> v_Shaders::readFile(const std::string& filename)
	{
		//start reading the shader at the end of the file and read it in binary
		std::ifstream shaderFile(filename, std::ios::ate | std::ios::binary); 
		if (!shaderFile.is_open())
		{
			throw std::runtime_error("Failed to open Shader file"); 
		}
		//returns current position in the file (starts at the due to std::ios::ate and so gives us the size of the shader
		size_t fileSize = (size_t)shaderFile.tellg();
		std::vector<char> buffer(fileSize); 

		//go to the beginning of the shader
		shaderFile.seekg(0);

		//read the file and input the data into our buffer
		shaderFile.read(buffer.data(), fileSize); 

		shaderFile.close(); 

		return buffer; 
	}

	VkShaderModule v_Shaders::createShaderModule(const std::vector<char>& shaderCode)
	{
		VkShaderModule module; 
		return module; 
	}
}