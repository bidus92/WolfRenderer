#pragma once
#include "vulkan/vulkan.h"
#include "SDL3/SDL.h"
#include "../v_QueueFamilies/v_QueueFamilies.h"
#include "v_ImageViews/v_ImageViews.h"


namespace WolfRenderer
{
	struct swapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class WLFR_API v_SwapChain
	{
	public:
		v_SwapChain(); 
		~v_SwapChain();
		void validateAndPopulate(const VkPhysicalDevice& device, SDL_Window* window, const VkSurfaceKHR& surface);
		//NOTE: add queueFamilyIndices argument to get necessary details
		void createSwapChainAndImageViews(const VkDevice& device, const VkSurfaceKHR& surface, v_QueueFamilies& queueFamilies);
		const char** ptrToRequiredSwapChainExtensions() { return deviceExtensions.data(); }
		std::vector<const char*> getRequiredSwapChainExtensions() {return deviceExtensions;}
		void destroySwapchain(VkDevice device);

	private:
		bool validateSwapChain(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);
	private:
		//like the validation layer, this special macro inputs all needed SwapChain extensions for Vulkan
		std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };


	private:
		VkSwapchainKHR swapChain; //the swapchain handle to use for creation


		//holds surface capabilities, surface format details, and presentation modes available for use in swapchain
		swapChainSupportDetails supportDetails; 

		VkSurfaceFormatKHR surfaceFormat; 
		VkPresentModeKHR presentMode; 
		VkExtent2D swapExtent; 
		uint32_t imageCount; 

		std::vector<VkImage> swapChainImages; 

		v_ImageViews imageViews; 
		
	private:
		swapChainSupportDetails checkSwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		//NOTE: WHERE WE CAN ENABLE V_SYNC IN OUR GAME ENGINE DEPENDING ON WHAT PRESENTATION MODE IS ENABLED
		VkPresentModeKHR chooseSwapPresentationMode(const std::vector<VkPresentModeKHR>& availablePresentationModes);

		//Resolution of SwapChain images
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities, SDL_Window* window);
	};
}


