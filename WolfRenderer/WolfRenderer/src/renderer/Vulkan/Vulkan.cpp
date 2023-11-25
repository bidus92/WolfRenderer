#include "pch.h"

#include "Vulkan.h"
#include "string.h"

#include "core/core.h"
#include "platform/Windows/WindowsWindow.h"

namespace WolfRenderer
{
	
	Vulkan::Vulkan()
	{

	}

	Vulkan::~Vulkan()
	{
		
	}
	Vulkan& Vulkan::Get()
	{
		static Vulkan s_Instance{};
		return s_Instance;
	}


	void Vulkan::createVulkan(SDL_Window* window)
	{
		Get().init_Vulkan(window);
	}
	
	void Vulkan::init_Vulkan(SDL_Window* window)
	{

		m_WinSurface.createWinSurface(window, m_Instance.getInstance(), m_WinSurface.getWinSurface());

		m_Devices.initialize(m_Instance.getInstance(), window, m_Debugger, m_WinSurface.getWinSurface());
	}

	void Vulkan::draw()
	{
		
		for (int currentFrame = 0; currentFrame < MAX_FRAMES_IN_FLIGHT; currentFrame++)
		{
	        vkWaitForFences(m_Devices.getLogicalDevice(), 1, m_Devices.getFencePtr(currentFrame), VK_TRUE, UINT64_MAX);

			uint32_t imageIndex; 
			VkResult result = vkAcquireNextImageKHR(m_Devices.getLogicalDevice(), m_Devices.getSwapchain(), UINT64_MAX, m_Devices.getImageSemaphore(currentFrame), VK_NULL_HANDLE, &imageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				m_Devices.swapChainInterface().recreateSwapChain(m_Devices.getLogicalDevice(),
					                                             m_Devices.swapChainInterface().getSwapChainImageExtent(),
					                                             m_WinSurface.getWinSurface(), 
					                                             m_Devices.queueFamilyInterface(),
					                                             m_Devices.imageViewInterface(), 
					                                             m_Devices.swapChainInterface().getSwapChainImages(), 
					                                             m_Devices.frameBufferInterface(), 
					                                             m_Devices.getGraphicsPipeline().getRenderPass());
				return;
			}

			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				throw std::runtime_error("Failed to acquire Vulkan Swapchain Image!\n");
			}
			vkResetCommandBuffer(m_Devices.getCommandBuffer(currentFrame), 0); 

			m_Devices.recordCommandBuffer(currentFrame);

			m_Devices.draw(imageIndex, currentFrame);
           
			vkResetFences(m_Devices.getLogicalDevice(), 1, m_Devices.getFencePtr(currentFrame));
			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT; 
		}
		
		

	}

	void Vulkan::drawFrame()
	{
		Get().draw();
	}

	void Vulkan::closeVulkan()
	{
		Get().destroy_Vulkan();
		std::cout << "Vulkan successfully destroyed!\n";
	}

	void Vulkan::destroy_Vulkan()
	{
		vkDeviceWaitIdle(m_Devices.getLogicalDevice());
		m_Devices.destroyLogicalDevice();
		m_Debugger.destroyDebugger(m_Instance.getInstance(), m_Debugger.getDebugMessenger());
		m_WinSurface.destroyWinSurface(m_Instance.getInstance(), m_WinSurface.getWinSurface());
		m_Instance.destroyInstance();
		std::cout << "Vulkan Instance successfully destroyed!\n";
	}


}
