#pragma once
#include "vulkan/vulkan.h"




namespace WolfRenderer
{

	class WLFR_API v_Framebuffer
	{
	public:
		v_Framebuffer(); 
		~v_Framebuffer(); 
		void createFramebuffers(const VkDevice& logicalDevice,
			                    const std::vector<VkImageView> theImageViews,
			                    const uint32_t& imageExtentWidth,
			                    const uint32_t& imageExtentHeight,
			                    const VkRenderPass& renderPass);

		void destroyFramebuffers(const VkDevice& logicalDevice); 

		std::vector<VkFramebuffer> getFrameBuffers() const { return framebuffers; }
	private:
		std::vector<VkFramebuffer> framebuffers; 
	};


}