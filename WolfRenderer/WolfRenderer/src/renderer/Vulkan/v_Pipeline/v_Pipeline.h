#pragma once
#include "../v_Shaders/v_Shaders.h"
#include "../v_Devices/v_SwapChain/v_SwapChain.h"

namespace WolfRenderer
{
	class WLFR_API v_Pipeline
	{
	public: 
		v_Pipeline(); 
		~v_Pipeline(); 

		void createGraphicsPipeline(const VkDevice& logicalDevice, v_SwapChain& swapchain);
		void destroyPipeline(const VkDevice& logicalDevice); 
		VkRenderPass getRenderPass() const { return renderPass; }
		VkPipeline getGraphicsPipeline() const { return graphicsPipeline; }
		VkViewport getViewport() const { return viewport; }
		VkRect2D getScissor() const { return scissor; }
		static VkRenderPass recreateRenderPass(const VkDevice& logicalDevice, v_SwapChain& swapchain, VkRenderPass& theRenderPass);
	private:
		//NOTE: Working Directory references our sandbox/startup project (in this case, it's the project dir of Sandbox) 
		v_Shaders theShaders{ "../WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/spirv/vert.spv"  , "../WolfRenderer/WolfRenderer/src/renderer/Vulkan/v_Shaders/Shaders/spirv/frag.spv" };
		std::vector<VkDynamicState> dynamicStates{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkRenderPass renderPass; 
		VkPipelineLayout graphicsPipelineLayout;
		VkPipeline graphicsPipeline; 
		
		VkViewport viewport; 
		VkRect2D scissor;
	private:
		void createRenderPass(const VkDevice& logicalDevice,  v_SwapChain& swapchain);
	};
}