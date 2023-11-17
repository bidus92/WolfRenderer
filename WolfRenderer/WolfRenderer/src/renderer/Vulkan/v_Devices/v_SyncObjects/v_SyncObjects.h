#pragma once

#include <vulkan/vulkan.h>
#include "../../v_Globals.h"



namespace WolfRenderer
{
	class WLFR_API v_SyncObjects
	{
	public:
		v_SyncObjects();
		~v_SyncObjects();
		void createSyncObjects(const VkDevice& logicalDevice);
		void destroySyncObjects(const VkDevice& logicalDevice);
		VkFence* getPtrToFences(int& currentFrame) { return &inFlightFences[currentFrame]; }
		VkSemaphore getImageSemaphore(int& currentFrame) const { return imageAvailableSemaphores[currentFrame]; }
		VkSemaphore getRenderFinishedSemaphore(int& currentFrame) const { return renderFinishedSemaphores[currentFrame]; }
		VkFence getInFlightFence(int& currentFrame) const { return inFlightFences[currentFrame]; }
	private:
		std::vector<VkSemaphore> imageAvailableSemaphores{ MAX_FRAMES_IN_FLIGHT };
		std::vector<VkSemaphore> renderFinishedSemaphores{ MAX_FRAMES_IN_FLIGHT };
		std::vector<VkFence> inFlightFences{ MAX_FRAMES_IN_FLIGHT };
		//VkSemaphore imageAvailableSemaphore;
		//VkSemaphore renderFinishedSemaphore;
		//VkFence inFlightFence;	
	};
}