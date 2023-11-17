#pragma once
#include <vulkan/vulkan.h>
#include "../v_QueueFamilies/v_QueueFamilies.h"



namespace WolfRenderer
{
	class WLFR_API v_CommandPool
	{
	public:
		v_CommandPool(); 
		~v_CommandPool(); 
		void createCommandPool(const VkDevice& logicalDevice, const QueueFamilyIndices& queueFamilies);
		void destroyCommandPool(const VkDevice& logicalDevice);
		VkCommandPool getCommandPool() const { return commandPool; }
	private:
		VkCommandPool commandPool; 
	};
}
