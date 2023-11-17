#pragma once
#include "vulkan/vulkan.h"
#include "SDL3/SDL.h"
#include "../v_QueueFamilies/v_QueueFamilies.h"
#include "../v_ImageViews/v_ImageViews.h"
#include "../v_Framebuffer/v_Framebuffer.h"


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
		VkSwapchainKHR createSwapChain(const VkDevice& device, const VkSurfaceKHR& surface, v_QueueFamilies queueFamilies);
		
		VkSwapchainKHR recreateSwapChain(const VkDevice& logicalDevice,
			                   const VkExtent2D& swapExtent,
			                   const VkSurfaceKHR& surface, 
			                   v_QueueFamilies queueFamilies, 
			                   v_ImageViews imageViews, 
			                   const std::vector<VkImage>& theSwapChainImages, 
			                   v_Framebuffer framebuffers, 
			                   VkRenderPass theRenderPass);

		void destroySwapchain(VkDevice device);


    //GETTERS
	public:		
		VkSwapchainKHR getSwapchainHandle() const { return swapChain; }
		const char** ptrToRequiredSwapChainExtensions() { return deviceExtensions.data(); }
		std::vector<const char*> getRequiredSwapChainExtensions() {return deviceExtensions;}
		VkExtent2D getSwapChainImageExtent() const { return swapExtent; }
		uint32_t getImageExtentWidth() const { return swapExtent.width; }
		uint32_t getImageExtentHeight() const { return swapExtent.height; }
		std::vector<VkImage> getSwapChainImages() const { return swapChainImages; }
		VkFormat getImageFormat() const { return surfaceFormat.format; }
		VkSurfaceFormatKHR getSurfaceFormat() const { return surfaceFormat; }

	//SWAPCHAIN VALIDATION
	private:
		bool validateSwapChain(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
		bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);
	private:
		//like the validation layer, this special macro inputs all needed SwapChain extensions for Vulkan
		std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };


	private:
		VkSwapchainKHR swapChain;


		//holds surface capabilities, surface format details, and presentation modes available for use in swapchain
		swapChainSupportDetails supportDetails; 

		VkSurfaceFormatKHR surfaceFormat; 
		VkPresentModeKHR presentMode; 
		VkExtent2D swapExtent; 
		uint32_t imageCount; 

		std::vector<VkImage> swapChainImages; 

		
	private:
		swapChainSupportDetails checkSwapChainSupportDetails(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		//NOTE: WHERE WE CAN ENABLE V_SYNC IN OUR GAME ENGINE DEPENDING ON WHAT PRESENTATION MODE IS ENABLED
		VkPresentModeKHR chooseSwapPresentationMode(const std::vector<VkPresentModeKHR>& availablePresentationModes);

		//Resolution of SwapChain images
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities, SDL_Window* window);
	};
}


