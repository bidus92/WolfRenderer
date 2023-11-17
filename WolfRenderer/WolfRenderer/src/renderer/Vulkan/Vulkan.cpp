#include "pch.h"

#include "Vulkan.h"
#include "string.h"

#include "core/core.h"
#include "platform/Windows/WindowsWindow.h"

namespace WolfRenderer
{
	
	Vulkan::Vulkan()
	{
		this->m_AvailableExtensions = getAvailableExtensions();
		this->m_RequiredExtensionNames = getRequiredExtensions();
	}

	Vulkan::~Vulkan()
	{
		
	}
	Vulkan& Vulkan::Get()
	{
		static Vulkan s_Instance{};
		return s_Instance;
	}


	VkInstance Vulkan::getInstance()
	{
		return Get().retrieveTheInstance();
	}

	VkDebugUtilsMessengerEXT Vulkan::getTheDebugger()
	{
		return Get().debugger.getDebugMessenger(); 
	}

	std::vector<VkExtensionProperties> Vulkan::getAvailableExtensions()
	{
		//TODO: edit first nullptr when validation layers are implemented
		vkEnumerateInstanceExtensionProperties(nullptr, &m_AvailableExtensionCount, nullptr);

		std::vector<VkExtensionProperties> extensionProperties(m_AvailableExtensionCount);
		//get extension properties of ALL available extensions on our OS
		vkEnumerateInstanceExtensionProperties(nullptr, &m_AvailableExtensionCount, extensionProperties.data());

		return extensionProperties;
	}


	//stores names of Vulkan instance extensions 
	std::vector<const char*> Vulkan::getRequiredExtensions()
	{
		//+1 reserved for event of debugger usage; will be modified later in optimization
		std::vector<const char*> extensionNames;

		if (debugger.enableLayers)
		{
            extensionNames.reserve(m_AvailableExtensionCount + 1);
            extensionNames.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		else
		{
			extensionNames.reserve(m_AvailableExtensionCount);
		}
	
		for (uint32_t i = 0; i < m_AvailableExtensionCount; i++)
		{
			const char* name = m_AvailableExtensions[i].extensionName;
			extensionNames.emplace_back(name);
		}
		//use these for SDL_Vulkan_CreateSurface
		WLFR_CORE_ASSERT(SDL_Vulkan_GetInstanceExtensions(&m_RequiredExtensionCount, m_RequiredExtensionNames.data()), "Failed to get required Vulkan extensions for SDL");
		

		return extensionNames; 

		//identifies our required extensions for Vulkan to work with SDL	
		 

	}


	void Vulkan::createVulkan(SDL_Window* window)
	{
		Get().init_Vulkan(window);
	}
	void Vulkan::createInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Sandbox";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "WolfRenderer";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 261.1);

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;


		createInfo.enabledExtensionCount = m_RequiredExtensionNames.size();
		createInfo.ppEnabledExtensionNames = m_RequiredExtensionNames.data();
		createInfo.pNext = &debugger.debuggerCreateInfo;


		if (debugger.enableLayers)
		{
			createInfo.enabledLayerCount = debugger.getEnabledLayerCount();
			createInfo.ppEnabledLayerNames = debugger.ptrToValidationLayers();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;
		}


		//TODO: establish allocator callback for memory optimization purposes.
		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);

		//TODO(Possible): implement into the event system! 
		if (result == VK_SUCCESS)
		{
			std::cout << "Vulkan initialization successful!\n";
		}
	}
	void Vulkan::init_Vulkan(SDL_Window* window)
	{
		createInstance();

		if (debugger.enableLayers)
		{
			std::cout << "Validation Layers Enabled!\n";
			debugger.setupDebugger(m_Instance, debugger.getDebuggerCreateInfo(), debugger.getDebugMessengerPtr());
		}
		else
		{
			std::cout << "Validation Layers Disabled!\n";
		}

		winSurface.createWinSurface(window, m_Instance, winSurface.getWinSurface());

		devices.initialize(m_Instance, window, debugger, winSurface.getWinSurface());
	}

	void Vulkan::draw()
	{
		
		for (int currentFrame = 0; currentFrame < MAX_FRAMES_IN_FLIGHT; currentFrame++)
		{
	        vkWaitForFences(devices.getLogicalDevice(), 1, devices.getFencePtr(currentFrame), VK_TRUE, UINT64_MAX);

			uint32_t imageIndex; 
			VkResult result = vkAcquireNextImageKHR(devices.getLogicalDevice(), devices.getSwapchain(), UINT64_MAX, devices.getImageSemaphore(currentFrame), VK_NULL_HANDLE, &imageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				devices.swapChainInterface().recreateSwapChain(devices.getLogicalDevice(), 
					                                           devices.swapChainInterface().getSwapChainImageExtent(), 
					                                           winSurface.getWinSurface(), 
					                                           devices.queueFamilyInterface(), devices.imageViewInterface(), devices.swapChainInterface().getSwapChainImages(), devices.frameBufferInterface(), devices.getGraphicsPipeline().getRenderPass());
				return;
			}

			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				throw std::runtime_error("Failed to acquire Vulkan Swapchain Image!\n");
			}
			vkResetCommandBuffer(devices.getCommandBuffer(currentFrame), 0); 

			devices.recordCommandBuffer(currentFrame);

			devices.draw(imageIndex, currentFrame);
           
			vkResetFences(devices.getLogicalDevice(), 1, devices.getFencePtr(currentFrame));
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
		vkDeviceWaitIdle(devices.getLogicalDevice());
		devices.destroyLogicalDevice(); 
		debugger.destroyDebugger(m_Instance);
		winSurface.destroyWinSurface(m_Instance, winSurface.getWinSurface()); 
		vkDestroyInstance(m_Instance, nullptr);
		std::cout << "Vulkan Instance successfully destroyed!\n";
	}


	VkInstance Vulkan::retrieveTheInstance()const
	{
		return m_Instance; 
	}

}
