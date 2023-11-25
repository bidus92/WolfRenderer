#include "pch.h"
#include "v_Devices.h"
#include "../Vulkan.h"

#include <map>

//TODO: create arbitrary parameter for v_Devices to add functionality...
//...but also make it so parent constructor of queue families are not automatically called 
namespace WolfRenderer
{
	v_Devices::v_Devices() :
		m_PhysicalDeviceCount(0)
	{
		
	}

	void v_Devices::initialize(VkInstance instance, SDL_Window* window, v_Debugger& theDebugger, VkSurfaceKHR theSurface)
	{
		m_AvailablePhysicalDevices = locatePhysicalDevices(instance);
		selectPhysicalDevice(); 
		m_QueueFamilies.acquire(this->m_PhysicalDevice, theSurface);
		m_SwapChain.validateAndPopulate(this->m_PhysicalDevice, window, theSurface);
		createLogicalDevice(theDebugger);
		std::cout << "Physical and Logical Devices successfuly set up!\n";


		//TODO: set these to be their own functionality outside of device initalization 
		m_SwapChain.createSwapChain(m_LogicalDevice, theSurface, m_QueueFamilies);
		m_ImageViews.createImageViews(m_LogicalDevice, m_SwapChain.getSwapChainImages(), m_SwapChain.getSurfaceFormat());
		m_GraphicsPipeline.createGraphicsPipeline(m_LogicalDevice, m_SwapChain);
		m_Framebuffer.createFramebuffers(m_LogicalDevice, m_ImageViews.getImageViews(), m_SwapChain.getImageExtentWidth(), m_SwapChain.getImageExtentHeight(), m_GraphicsPipeline.getRenderPass());
		m_CommandPool.createCommandPool(m_LogicalDevice, m_QueueFamilies.getQueueFamilyIndices());

		m_CommandBuffer.createCommandBuffer(m_LogicalDevice, m_CommandPool.getCommandPool()); 

		m_SyncObjects.createSyncObjects(m_LogicalDevice); 
	}

	void v_Devices::draw(const uint32_t& imageIndex, int& currentFrame)
	{
			m_QueueFamilies.submitQueue(m_QueueFamilies.getGraphicsQueueHandle(), m_SyncObjects.getImageSemaphore(currentFrame), m_SyncObjects.getRenderFinishedSemaphore(currentFrame), m_SyncObjects.getInFlightFence(currentFrame), m_CommandBuffer.getCommandBufferPtr(currentFrame));
			m_QueueFamilies.presentTheQueue(m_SyncObjects.getImageSemaphore(currentFrame), m_QueueFamilies.getPresentQueueHandle(), m_SwapChain.getSwapchainHandle(), imageIndex);
	}

	v_Devices::~v_Devices()
	{
		
	}

	std::vector<VkPhysicalDevice> v_Devices::locatePhysicalDevices(VkInstance instance)
	{
		vkEnumeratePhysicalDevices(instance, &m_PhysicalDeviceCount, nullptr);
		//TODO: implement into event system 
		if (m_PhysicalDeviceCount == 0)
		{
			throw std::runtime_error("No GPUs available on current hardware to support Vulkan!\n");
		}

		std::vector<VkPhysicalDevice> devices(m_PhysicalDeviceCount);
		vkEnumeratePhysicalDevices(instance, &m_PhysicalDeviceCount, devices.data());

		return devices; 
	}

	//rating system to help determine which physical device best suitable 
	//NOTE: will evolve over development and engine needs
	int v_Devices::ratePhysicalDevice(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		int score = 0; 

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000; 
		}

		score += deviceProperties.limits.maxImageDimension2D;

		if (!deviceFeatures.geometryShader)
		{
			return 0; 
		}

		return score; 
		
	}

	//find GPU suitable for our needs
	bool v_Devices::isDeviceSuitable(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties; 
        vkGetPhysicalDeviceProperties(device, &deviceProperties); 

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);	
		

		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && 
			deviceFeatures.geometryShader;
	}

	void v_Devices::selectPhysicalDevice()
	{
		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : m_AvailablePhysicalDevices)
		{
			int score = ratePhysicalDevice(device);
			if (isDeviceSuitable(device))
			{
               candidates.insert(std::make_pair(score, device));
			}
		}

		if (candidates.rbegin()->first > 0)
		{
			m_PhysicalDevice = candidates.rbegin()->second; 
			vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_PhysicalDeviceProperties); 
			vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_PhysicalDeviceFeatures);
			std::cout << m_PhysicalDeviceProperties.deviceName << " has been chosen!\n";
		}


		if (m_PhysicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("Unable to find GPU for Vulkan usage\n");
		}

	}
	
	void v_Devices::createLogicalDevice(v_Debugger& theDebugger)
	{
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; 
		 
		//TODO: implement getter for queueCreateInfo struct
		createInfo.pQueueCreateInfos = m_QueueFamilies.ptrToQueueCreateInfo();
		createInfo.queueCreateInfoCount = m_QueueFamilies.getQueueCreateInfoSize();//static_cast<uint32_t>(this->m_QueueFamilies.getQueueCreateInfos().size());
		createInfo.pEnabledFeatures = &m_PhysicalDeviceFeatures;
		createInfo.enabledExtensionCount = m_SwapChain.getRequiredSwapChainExtensions().size();
		createInfo.ppEnabledExtensionNames = m_SwapChain.ptrToRequiredSwapChainExtensions();
		
		if (theDebugger.isValidationEnabled())
		{
			createInfo.enabledLayerCount = theDebugger.getEnabledLayerCount();
			createInfo.ppEnabledLayerNames = theDebugger.ptrToValidationLayers(); 
		}
		else
		{
			createInfo.enabledLayerCount = 0; 
		}

			if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to create Vulkan Logical Device!");
		}
		else
		{
			std::cout << "Vulkan logical device successfully created!\n";
		}

		m_QueueFamilies.getGraphicsQueue(m_LogicalDevice);
		m_QueueFamilies.getPresentationQueue(m_LogicalDevice);
	}

	void v_Devices::destroyLogicalDevice()
	{
		m_SyncObjects.destroySyncObjects(m_LogicalDevice); 
		m_CommandPool.destroyCommandPool(m_LogicalDevice); 
		m_Framebuffer.destroyFramebuffers(m_LogicalDevice); 
		m_GraphicsPipeline.destroyPipeline(m_LogicalDevice);
		m_ImageViews.destroyImageViews(m_LogicalDevice); 
		m_SwapChain.destroySwapchain(m_LogicalDevice); 
		vkDestroyDevice(m_LogicalDevice, nullptr);
		std::cout << "Logical device successfully destroyed!\n";
	}
	
}
