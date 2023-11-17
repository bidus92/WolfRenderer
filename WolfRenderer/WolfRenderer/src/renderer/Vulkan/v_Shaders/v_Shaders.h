#pragma once

#include "vulkan/vulkan.h"
#include <initializer_list>



namespace WolfRenderer 
{

	
	class WLFR_API v_Shaders
	{
	public:

		
		v_Shaders (const std::initializer_list<std::string>& shaderFiles);


		~v_Shaders(); 
		//Possible multithreading implementation for multiple shader reads at once 
		void readFiles(const std::vector<std::string>& shaderFiles); 
		void createShaderModules(const VkDevice& logicalDevice);
		VkPipelineShaderStageCreateInfo* shaderStages() { return shaderCreateInfos.data(); }
		uint32_t getNumShaderStages() const { return shaderCreateInfos.size(); }
		void destroyShaderModules(const VkDevice& logicalDevice);
	private:

		enum class SHADER_ID
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};
		//Temporary for demo purposes of proving functionlity
		
		struct st_Shader
		{
			VkShaderModule shaderModule; 
            std::vector<char> shaderByteFile;  
			SHADER_ID id = SHADER_ID::NONE; 
			VkPipelineShaderStageCreateInfo createInfo; 
		};

		std::vector<st_Shader> shaders; 
		//std::vector<VkShaderModule> shaderModules;
		const std::vector<std::string> theShaderFiles; 
		std::vector<VkPipelineShaderStageCreateInfo> shaderCreateInfos; 
		

	};

}
