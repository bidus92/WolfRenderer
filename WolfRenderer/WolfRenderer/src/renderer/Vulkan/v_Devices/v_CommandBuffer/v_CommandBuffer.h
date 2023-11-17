#pragma once


#include <vulkan/vulkan.h>
#include "../../v_Globals.h"

namespace WolfRenderer
{
	class WLFR_API v_CommandBuffer
	{
	public:
		v_CommandBuffer(); 
		~v_CommandBuffer(); 
		void createCommandBuffer(const VkDevice& logicalDevice, const VkCommandPool& theCommandPool);
		void recordCommandBuffer(const VkDevice& logicalDevice,
			                     const VkRenderPass& renderPass,
			                     const std::vector<VkFramebuffer>& framebuffers,
			                     const VkExtent2D& swapChainImageExtent, const VkPipeline& pipeline,
			                     uint32_t imageIndex, 
			                     const VkViewport& viewport, 
			                     const VkRect2D& scissor, int& currentFrame);
		void setViewportAndScissor(const VkViewport& viewport, const VkRect2D& scissor, int& currentFrame);
		VkCommandBuffer getCommandBufferHandle(int& currentFrame) const { return commandBuffers[currentFrame]; }
		VkCommandBuffer* getCommandBufferPtr(int& currentFrame) { return &commandBuffers[currentFrame]; }
		void draw(int& currentFrame);
	private:
		std::vector<VkCommandBuffer> commandBuffers{ MAX_FRAMES_IN_FLIGHT };
	};
}