#include "pch.h"
#include "v_SyncObjects.h"



namespace WolfRenderer
{
	v_SyncObjects::v_SyncObjects() 
	{

	}
	v_SyncObjects::~v_SyncObjects()
	{

	}

	void v_SyncObjects::createSyncObjects(const VkDevice& logicalDevice)
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo{}; 
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; 
		semaphoreCreateInfo.pNext = nullptr; 
		semaphoreCreateInfo.flags = 0; 

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO; 
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; 
		fenceCreateInfo.pNext = nullptr; 

		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			   vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			   vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Vulkan Sync Object creation unsuccessful!\n"); 
			}
		}


		std::cout << "Vulkan Sync Object creation successful!\n";
	}

	void v_SyncObjects::destroySyncObjects(const VkDevice& logicalDevice)
	{
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
		}


		std::cout << "Vulkan Sync Objects successfully destroyed!\n";
	}
}