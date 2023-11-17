#include "pch.h"
#include "v_Framebuffer.h"
#include "../v_ImageViews/v_ImageViews.h"

namespace WolfRenderer
{
	v_Framebuffer::v_Framebuffer()
	{

	}
	v_Framebuffer::~v_Framebuffer()
	{

	}
	void v_Framebuffer::createFramebuffers(const VkDevice& logicalDevice,
		const std::vector<VkImageView> theImageViews,
		const uint32_t& imageExtentWidth,
		const uint32_t& imageExtentHeight,
		const VkRenderPass& renderPass)
	{
		framebuffers.resize(theImageViews.size());

		for (size_t i = 0; i < framebuffers.size(); i++)
		{
			//VkImageView attachments[] = { theImageViews[i] };


			VkFramebufferCreateInfo framebufferCreateInfo{};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = renderPass;
			framebufferCreateInfo.attachmentCount = 1;
			framebufferCreateInfo.pAttachments = &theImageViews[i];
			framebufferCreateInfo.width = imageExtentWidth;
			framebufferCreateInfo.height = imageExtentHeight;
			framebufferCreateInfo.layers = 1;

			if (vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Vulkan Framebuffer creation unsuccessful!");
			}
		}

		std::cout << "Vulkan Framebuffer creation successful!\n";
	};

	void v_Framebuffer::destroyFramebuffers(const VkDevice& logicalDevice)
	{
		for (const auto& framebuffer : framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr); 
		}
		
		std::cout << "Vulkan Framebuffers successfully destroyed!\n";
	};
}