#pragma once


#include "vulkan/vulkan.h"
#include "SDL3/SDL.h"




namespace WolfRenderer
{
	class WLFR_API v_ImageViews
	{
	public:
		v_ImageViews(); 
		~v_ImageViews();
		void createImageViews(const VkDevice& device, const std::vector<VkImage>& theImages, const VkSurfaceFormatKHR& format);

		void destroyImageViews(const VkDevice& device);
		std::vector<VkImageView> getImageViews() const { return swapChainImageViews; }
	private:
		std::vector<VkImageView> swapChainImageViews;
		
	};
}