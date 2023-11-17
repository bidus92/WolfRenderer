#include "pch.h"
#include "v_CommandBuffer.h"


namespace WolfRenderer
{
	v_CommandBuffer::v_CommandBuffer() 
	{

	}
	v_CommandBuffer::~v_CommandBuffer()
	{

	}

	void v_CommandBuffer::createCommandBuffer(const VkDevice& logicalDevice, const VkCommandPool& theCommandPool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = theCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //can be submitted to queues directly but not called from other command buffers
		allocInfo.commandBufferCount = commandBuffers.size();

		if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate for Vulkan Command Buffers\n");
		}

		std::cout << "Command Buffer creation/allocation successful!\n";
	}

	void v_CommandBuffer::recordCommandBuffer(const VkDevice& logicalDevice,
		const VkRenderPass& renderPass,
		const std::vector<VkFramebuffer>& framebuffers,
		const VkExtent2D& swapChainImageExtent, const VkPipeline& pipeline,
		uint32_t imageIndex, 
		const VkViewport& viewport, 
		const VkRect2D& scissor, int& currentFrame)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		//which state to inherit from the calling primary command buffer (if it were a secondary) 
		beginInfo.pInheritanceInfo = nullptr;	

		if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Vulkan Command Buffer failed to begin!\n");
		}

		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.framebuffer = framebuffers[imageIndex];
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = swapChainImageExtent;

		VkClearValue clearColor = { {{ 0.f, 0.f, 0.f, 1.f }} }; 
		renderPassBeginInfo.clearValueCount = 1; 
		renderPassBeginInfo.pClearValues = &clearColor; 

		vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		setViewportAndScissor(viewport, scissor, currentFrame); 
		draw(currentFrame);
		vkCmdEndRenderPass(commandBuffers[currentFrame]);

		if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to end Vulkan Command Buffer!\n"); 
		}
		//std::cout << "Vulkan Command Buffer successfully recorded!\n";
	}
	void v_CommandBuffer::setViewportAndScissor(const VkViewport& viewport, const VkRect2D& scissor, int& currentFrame)
	{
		vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);
	}
	void v_CommandBuffer::draw(int& currentFrame)
	{
		vkCmdDraw(commandBuffers[currentFrame], 3, 1, 0, 0);
	}
}