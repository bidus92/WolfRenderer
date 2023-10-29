#pragma once

#include "vulkan/vulkan.h"




namespace WolfRenderer 
{
	class WLFR_API v_Shaders
	{
	public:
		v_Shaders(); 
		~v_Shaders(); 
		inline std::vector<char> readFile(const std::string& filename); 
		VkShaderModule createShaderModule(const std::vector<char>& shaderCode); 
	private:

	};

}
