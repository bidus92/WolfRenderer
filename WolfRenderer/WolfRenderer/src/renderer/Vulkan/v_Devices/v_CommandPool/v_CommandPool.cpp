#include "pch.h"
#include "v_CommandPool.h"



namespace WolfRenderer
{
	v_CommandPool::v_CommandPool()
	{

	}
	v_CommandPool::~v_CommandPool()
	{

	}
	void v_CommandPool::createCommandPool(const VkDevice& logicalDevice, const QueueFamilyIndices& queueFamilies)
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo{};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO; 
		commandPoolCreateInfo.queueFamilyIndex = queueFamilies.graphicsFamily.value(); 
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; 
		commandPoolCreateInfo.pNext = nullptr; 

		if (vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to create Vulkan Command Pool!\n");
		}

		std::cout << "Vulkan Command Pool creation successful!\n"; 
	}

	

	void v_CommandPool::destroyCommandPool(const VkDevice& logicalDevice)
	{
		vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
		std::cout << "Vulkan command pool successfully destroyed!\n";
	}
}