#include "pch.h"
#include "v_Shaders.h"
#include <fstream>


namespace WolfRenderer
{
	//TODO: Put all 
	v_Shaders::v_Shaders(const std::initializer_list<std::string>& shaderFiles) :
		theShaderFiles(shaderFiles)
	{ 
		shaders.resize(theShaderFiles.size()); 
		readFiles(theShaderFiles); 
	}

	v_Shaders::~v_Shaders()
	{

	}

	void v_Shaders::readFiles(const std::vector<std::string>& shaderFiles)
	{
		for (int i = 0; i < shaders.size(); i++)
		{
			//start reading the shader at the end of the file and read it in binary
			std::ifstream file(shaderFiles[i], std::ios::binary | std::ios::ate);
			if (!file.is_open() && !file.good())
			{
				throw std::runtime_error("Failed to open Shader file");
			}
		
			if (shaderFiles[i].find("vert") != std::string::npos)
			{
				shaders[i].id = SHADER_ID::VERTEX;
			}

			else if (shaderFiles[i].find("frag") != std::string::npos)
			{
				shaders[i].id = SHADER_ID::FRAGMENT;
			}


			//returns current position in the file (starts at the due to std::ios::ate and so gives us the size of the shader
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			//go to the beginning of the shader
			file.seekg(0);

			//read the file and input the data into our buffer
			file.read(buffer.data(), fileSize);

			file.close();

			shaders[i].shaderByteFile = buffer;
		}
		

	}

	void v_Shaders::createShaderModules(const VkDevice& logicalDevice)
	{
		for (auto& shader : shaders)
		{
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = shader.shaderByteFile.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(shader.shaderByteFile.data());
			createInfo.pNext = nullptr;

			if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shader.shaderModule) != VK_SUCCESS)
			{
				throw std::runtime_error("Unable to create Vulkan shader module!\n");
			}

			if (shader.id == SHADER_ID::VERTEX)
			{
				VkPipelineShaderStageCreateInfo vertexShaderCreateInfo; 
				vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO; 
				vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; 
				vertexShaderCreateInfo.module = shader.shaderModule; 
				vertexShaderCreateInfo.pName = "main"; 
				vertexShaderCreateInfo.flags = 0;
				vertexShaderCreateInfo.pNext = nullptr;
				vertexShaderCreateInfo.pSpecializationInfo = nullptr;	

				shader.createInfo = vertexShaderCreateInfo; 

				shaderCreateInfos.push_back(vertexShaderCreateInfo);
			}
			if (shader.id == SHADER_ID::FRAGMENT)
			{
				VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo;
				fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				fragmentShaderCreateInfo.module = shader.shaderModule;
				fragmentShaderCreateInfo.pName = "main";
				fragmentShaderCreateInfo.flags = 0;
				fragmentShaderCreateInfo.pNext = nullptr;
				fragmentShaderCreateInfo.pSpecializationInfo = nullptr;

				shader.createInfo = fragmentShaderCreateInfo;

				shaderCreateInfos.push_back(fragmentShaderCreateInfo);
			}
		}
		std::cout << "Vulkan Shader creation successful!\n";

	}

	void v_Shaders:: destroyShaderModules(const VkDevice& logicalDevice)
	{
		for (const auto& shader : shaders)
		{
			vkDestroyShaderModule(logicalDevice, shader.shaderModule, nullptr); 
		}
		std::cout << "Vulkan Shader Modules successfully destroyed!\n";
	}
}