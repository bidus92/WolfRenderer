#pragma once

#include "vulkan/vulkan.h"
#include "SDL3/SDL_vulkan.h"
#include <optional>




namespace WolfRenderer
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
	};

	class WLFR_API v_QueueFamilies
	{
	public:
		v_QueueFamilies(); 
		~v_QueueFamilies(); 
		void acquire(VkPhysicalDevice device);
		VkDeviceQueueCreateInfo getQueueCreateInfo() { return queueCreateInfo; }
		VkDeviceQueueCreateInfo* ptrToQueueCreateInfo() { return &queueCreateInfo; }
		void retrieveQueueHandle(VkDevice device, uint32_t queueIndex, VkQueue queueHandle); 


	private:
		std::vector<VkQueueFamilyProperties> queueFamilyProperties; 
		QueueFamilyIndices indices;
		VkDeviceQueueCreateInfo queueCreateInfo; 
		VkQueue graphicsQueue; 

	private:
		std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device);
        bool queueFamiliesFound(); 
        QueueFamilyIndices findQueueFamilies();
		VkDeviceQueueCreateInfo inputQueueCreateInfo();
		
	};
}


