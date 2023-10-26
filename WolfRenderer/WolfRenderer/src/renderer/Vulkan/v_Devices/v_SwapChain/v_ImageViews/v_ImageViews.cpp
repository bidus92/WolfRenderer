
#include "pch.h"


#include "v_ImageViews.h"



namespace WolfRenderer
{
	v_ImageViews::v_ImageViews()
	{

	}
	v_ImageViews::~v_ImageViews()
	{

	}
	void v_ImageViews::createImageViews(const VkDevice& device, const std::vector<VkImage>& theImages, const VkSurfaceFormatKHR& surfaceFormat)
	{
		swapChainImageViews.resize(theImages.size()); 

		//TODO: implement multithreading for imageView creation!
		for (size_t i = 0; i < theImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO; 
			createInfo.image = theImages[i]; 
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = surfaceFormat.format;

			//allows swizzle of color channels to create different color images
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; 
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			//purpose of the image
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			//mipmap details
			createInfo.subresourceRange.baseMipLevel = 0; 
			createInfo.subresourceRange.levelCount = 1; 
			createInfo.subresourceRange.baseArrayLayer = 0; 
			createInfo.subresourceRange.layerCount = 1; 

			if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create Vulkan ImageViews!\n");
			}

		}

		std::cout << "Vulkan ImageView creations successful!\n";
		
	}

	void v_ImageViews::destroyImageViews(const VkDevice& device)
	{
		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}

		std::cout << "Vulkan ImageViews successfully destroyed!\n";
	}
}