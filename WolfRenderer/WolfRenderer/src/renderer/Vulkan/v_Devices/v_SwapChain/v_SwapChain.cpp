#include "pch.h"

#include "v_SwapChain.h"

#include <set>
#include <limits>
#include <SDL3/SDL_vulkan.h>

namespace WolfRenderer
{
	v_SwapChain::v_SwapChain()
	{

	}

	v_SwapChain::~v_SwapChain()
	{

	}

	//TODO: Multithreaded actions to take place with selected surfaceFormat, presentation mode, and swap extent
	void v_SwapChain::validateAndPopulate(const VkPhysicalDevice& device, SDL_Window* window, const VkSurfaceKHR& surface)
	{
		validateSwapChain(device, surface); 
		this->surfaceFormat = chooseSwapSurfaceFormat(this->supportDetails.formats);
		this->presentMode = chooseSwapPresentationMode(this->supportDetails.presentModes);
		this->swapExtent = chooseSwapExtent(this->supportDetails.capabilities, window);

		//acquire one more image than the minimum so we don't have to wait on driver operations to complete
//before taking in another image into the swapchain 
		this->imageCount = supportDetails.capabilities.minImageCount + 1;

		if (supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount)
		{
			this->imageCount = supportDetails.capabilities.maxImageCount;
		}
	}

	//TODO: Finish function tomorrow! 
	void v_SwapChain::createSwapChainAndImageViews(const VkDevice& device, const VkSurfaceKHR& surface, v_QueueFamilies& queueFamilies)
	{

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR; 
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount; 
		createInfo.imageFormat = surfaceFormat.format; 
		createInfo.imageColorSpace = surfaceFormat.colorSpace; 
		createInfo.imageExtent = swapExtent;
		//used for stereoscopic 3D applications
		createInfo.imageArrayLayers = 1; 
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; 

		uint32_t queuefamilyIndices[] = { queueFamilies.getQueueFamilyIndices().graphicsFamily.value(), queueFamilies.getQueueFamilyIndices().presentFamily.value() };
		if (queueFamilies.getQueueFamilyIndices().graphicsFamily != queueFamilies.getQueueFamilyIndices().presentFamily)
		{
			//TODO: create functionality to split queue families to use concurrent in the future
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; 
			createInfo.queueFamilyIndexCount = 2; 
			createInfo.pQueueFamilyIndices = queuefamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; 
			createInfo.queueFamilyIndexCount = 0; 
			createInfo.pQueueFamilyIndices = nullptr; 
		}

		createInfo.preTransform = supportDetails.capabilities.currentTransform; //apply a transformation to the image if so requested
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //Do we use the alpha channel to blend with other windows 
		createInfo.presentMode = presentMode; 
		createInfo.clipped = VK_TRUE; //clips unseen pixels 

		createInfo.oldSwapchain = VK_NULL_HANDLE; 

		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &this->swapChain) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to create Vulkan Swapchain!");
		}

		else
		{
			std::cout << "Vulkan swapchain creation successful!\n";
		}

		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr); 
		swapChainImages.resize(imageCount); 
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());


		imageViews.createImageViews(device, swapChainImages, surfaceFormat); 
		
	}

	bool v_SwapChain::validateSwapChain(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		this->checkDeviceExtensionSupport(device);

		this->supportDetails = checkSwapChainSupportDetails(device, surface);

		std::cout << "Vulkan SwapChain validated\n";
		
		return !supportDetails.formats.empty() && !supportDetails.presentModes.empty(); 
	}

	bool v_SwapChain::checkDeviceExtensionSupport(const VkPhysicalDevice& device)
	{
		uint32_t extensionCount; 
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr); 

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end()); 

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
			if (requiredExtensions.empty())
			{
				break; 
			}
		}
		if (!requiredExtensions.empty())
		{
			throw std::runtime_error("Required Vulkan SwapChain extensions not found");
		}

		
		return requiredExtensions.empty(); 

	}

	swapChainSupportDetails v_SwapChain::checkSwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		swapChainSupportDetails details; 

		//acquire capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities); 

		//acquire available formats (pixel format, color space, etc.) 


		uint32_t formatCount; 
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		

		if (formatCount != 0)
		{
			details.formats.resize(formatCount); 
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data()); 
		}

		uint32_t presentModeCount; 
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr); 


		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount); 
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details; 
	}

	//NOTE!: Potential to multithread this! Checks are all not connected to one another...async Implementation will happen 
	VkSurfaceFormatKHR v_SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat; 
			}
		}

		//TODO: add ranking system to select best available format system for SwapChain
		return availableFormats[0];
	}

	VkPresentModeKHR v_SwapChain::chooseSwapPresentationMode(const std::vector<VkPresentModeKHR>& availablePresentationModes)
	{
		for (const auto& availablePresentMode : availablePresentationModes)
		{
			//ideal for most modern hardware scenarios with the exception of mobile 
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode; 
			}
		}

		//Only mode guaranteed to be present with Vulkan implementations; Not optimal 
		return VK_PRESENT_MODE_FIFO_KHR; 
	}

	VkExtent2D v_SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities, SDL_Window* window)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent; 
		}
		else
		{
			int width, height; 

			SDL_GetWindowSizeInPixels(window, &width, &height); 

			VkExtent2D actualExtent =
			{
				static_cast<uint32_t> (width),
				static_cast<uint32_t> (height)
			};

			//clamps values between allowed minimum and maximum width/height of swapChain extent capababilities
			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void v_SwapChain::destroySwapchain(VkDevice device) 
	{ 
		imageViews.destroyImageViews(device); 
		vkDestroySwapchainKHR(device, swapChain, nullptr); 
		std::cout << "Vulkan Swapchain successfully destroyed!\n";
	}
}