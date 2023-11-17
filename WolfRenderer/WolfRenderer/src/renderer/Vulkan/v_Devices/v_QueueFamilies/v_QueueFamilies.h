#pragma once

#include "vulkan/vulkan.h"
#include "SDL3/SDL_vulkan.h"
#include <optional>



namespace WolfRenderer
{

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily; 
		//std::optional<uint32_t> transferFamily; 
		//std::optional<uint32_t> computeFamily; 
	};

	class WLFR_API v_QueueFamilies
	{
	public:
		v_QueueFamilies(); 
		~v_QueueFamilies(); 
		void acquire(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

		void retrieveQueueHandle(const VkDevice& device, const uint32_t& queueIndex, VkQueue queueHandle); 


		uint32_t getQueueCreateInfoSize() { return queueCreateInfos.size(); }
		VkDeviceQueueCreateInfo* ptrToQueueCreateInfo() { return queueCreateInfos.data(); }
		QueueFamilyIndices getQueueFamilyIndices() { return indices; }
		QueueFamilyIndices* ptrToQueueFamilyIndices() { return &indices; }
		void getGraphicsQueue(VkDevice logicalDevice);
		void getPresentationQueue(VkDevice logicalDevice);

	private:
		std::vector<VkQueueFamilyProperties> queueFamilyProperties; 
		QueueFamilyIndices indices;
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		VkQueue graphicsQueue; 
		VkQueue presentQueue;

	private:
		std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device);
        bool queueFamiliesFound(); 
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
		std::vector<VkDeviceQueueCreateInfo> inputQueueCreateInfos();
		float queuePriority = 1.f;
	};
}


