#pragma once

#include "core/core.h"

#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "v_QueueFamilies/v_QueueFamilies.h"
#include "../v_Debugger/v_Debugger.h"
#include "../v_Surface/v_Surface.h"
#include "v_SwapChain/v_SwapChain.h"
#include"../v_Pipeline/v_Pipeline.h"
#include "v_Framebuffer/v_Framebuffer.h"
#include "v_ImageViews/v_ImageViews.h"
#include "v_CommandPool/v_CommandPool.h"
#include "v_CommandBuffer/v_CommandBuffer.h"
#include "v_SyncObjects/v_SyncObjects.h"

namespace WolfRenderer
{
	class WLFR_API v_Devices
	{
		
	public:
		v_Devices(); 
		~v_Devices(); 
		void selectPhysicalDevice();
		void initialize (VkInstance instance, SDL_Window* window, v_Debugger& theDebugger, VkSurfaceKHR theSurface);
		void destroyLogicalDevice();
		VkFence* getFencePtr(int& currentFrame)  { return syncObjects.getPtrToFences(currentFrame); }

	public:
	    VkDevice getLogicalDevice() const { return logicalDevice; }
		VkSwapchainKHR getSwapchain() const { return swapChain.getSwapchainHandle(); }
		v_SwapChain swapChainInterface() const { return swapChain; }
        VkCommandBuffer getCommandBuffer(int& currentFrame) const { return  commandBuffer.getCommandBufferHandle(currentFrame); }
		v_QueueFamilies queueFamilyInterface() const { return queueFamilies; }
		v_ImageViews imageViewInterface() const { return imageViews; }
		v_Framebuffer frameBufferInterface() const { return framebuffer; }
		v_Pipeline getGraphicsPipeline() const { return graphicsPipeline; }
	public:	
		void draw(const uint32_t& imageIndex, int& currentFrame);

//TODO: MAKE BETTER INTERFACE WITH LESS SYNTAX
//functions to interface with Vulkan Singleton
	public:
		VkSemaphore getImageSemaphore(int& currentFrame) const { return syncObjects.getImageSemaphore(currentFrame); }
		VkSemaphore getRenderFinishedSemaphore(int& currentFrame) const { return syncObjects.getRenderFinishedSemaphore(currentFrame); }
		VkFence getInFlightFence(int& currentFrame) const { return syncObjects.getInFlightFence(currentFrame); }
		void recordCommandBuffer(int& currentFrame) { commandBuffer.recordCommandBuffer(logicalDevice, graphicsPipeline.getRenderPass(), framebuffer.getFrameBuffers(), swapChain.getSwapChainImageExtent(), graphicsPipeline.getGraphicsPipeline(), 0, graphicsPipeline.getViewport(), graphicsPipeline.getScissor(), currentFrame); }
//**Physical Device members and functions
	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; 
		uint32_t physicalDeviceCount; 
		std::vector<VkPhysicalDevice> availablePhysicalDevices;
		VkPhysicalDeviceProperties physicalDeviceProperties; 
		VkPhysicalDeviceFeatures physicalDeviceFeatures; 


	private:
		std::vector<VkPhysicalDevice> locatePhysicalDevices(VkInstance instance);
		int ratePhysicalDevice(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);


//**Misc. classes to interact with devices
	private:
		v_QueueFamilies queueFamilies;
		v_SwapChain swapChain; 
		v_Pipeline graphicsPipeline;	 
		v_Framebuffer framebuffer; 
		v_ImageViews imageViews; 


		v_CommandPool commandPool; 
		v_CommandBuffer commandBuffer;
		v_SyncObjects syncObjects;

//**Logical Device members and functions
	private:
		VkDevice logicalDevice;
	private:
		void createLogicalDevice(v_Debugger& theDebugger);


		
		
	};
}