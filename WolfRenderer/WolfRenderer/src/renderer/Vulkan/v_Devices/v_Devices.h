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
		VkFence* getFencePtr(int& currentFrame)  { return m_SyncObjects.getPtrToFences(currentFrame); }

	//**GETTERS
	public:
	    VkDevice getLogicalDevice() const { return m_LogicalDevice; }


		VkSwapchainKHR getSwapchain() const { return m_SwapChain.getSwapchainHandle(); }

		//TODO: MOVE TO VULKAN FILE!
		v_SwapChain swapChainInterface() const { return m_SwapChain; }
        VkCommandBuffer getCommandBuffer(int& currentFrame) const { return  m_CommandBuffer.getCommandBufferHandle(currentFrame); }
		v_QueueFamilies queueFamilyInterface() const { return m_QueueFamilies; }
		v_ImageViews imageViewInterface() const { return m_ImageViews; }
		v_Framebuffer frameBufferInterface() const { return m_Framebuffer; }
		v_Pipeline getGraphicsPipeline() const { return m_GraphicsPipeline; }
	public:	
		void draw(const uint32_t& imageIndex, int& currentFrame);

//TODO: MAKE BETTER INTERFACE WITH LESS SYNTAX
//functions to interface with Vulkan Singleton
	public:
		VkSemaphore getImageSemaphore(int& currentFrame) const { return m_SyncObjects.getImageSemaphore(currentFrame); }
		VkSemaphore getRenderFinishedSemaphore(int& currentFrame) const { return m_SyncObjects.getRenderFinishedSemaphore(currentFrame); }
		VkFence getInFlightFence(int& currentFrame) const { return m_SyncObjects.getInFlightFence(currentFrame); }
		void recordCommandBuffer(int& currentFrame) { m_CommandBuffer.recordCommandBuffer(m_LogicalDevice, m_GraphicsPipeline.getRenderPass(), m_Framebuffer.getFrameBuffers(), m_SwapChain.getSwapChainImageExtent(), m_GraphicsPipeline.getGraphicsPipeline(), 0, m_GraphicsPipeline.getViewport(), m_GraphicsPipeline.getScissor(), currentFrame); }
//**Physical Device members and functions
	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE; 
		uint32_t m_PhysicalDeviceCount; 
		std::vector<VkPhysicalDevice> m_AvailablePhysicalDevices;
		VkPhysicalDeviceProperties m_PhysicalDeviceProperties; 
		VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures; 

//**Physical Device selection functions
	private:
		std::vector<VkPhysicalDevice> locatePhysicalDevices(VkInstance instance);
		int ratePhysicalDevice(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);


//**Misc. classes to interact with devices
//TODO: Move setup to Vulkan class 
	private:
		v_QueueFamilies m_QueueFamilies;
		v_SwapChain m_SwapChain; 
		v_Pipeline m_GraphicsPipeline;	 
		v_Framebuffer m_Framebuffer; 
		v_ImageViews m_ImageViews; 

		v_CommandPool m_CommandPool; 
		v_CommandBuffer m_CommandBuffer;
		v_SyncObjects m_SyncObjects;

//**Logical Device members and functions
	private:
		VkDevice m_LogicalDevice;
	private:
		void createLogicalDevice(v_Debugger& theDebugger);


		
		
	};
}