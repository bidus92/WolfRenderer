#include "pch.h"
#include "v_Instance.h"

#include <SDL3/SDL_Vulkan.h>




namespace WolfRenderer
{
	v_Instance::v_Instance(v_Debugger& debugger, std::mutex& instanceCreationStatus, std::condition_variable& instanceCreationSuccessful)
	{
		std::lock_guard<std::mutex> lk(instanceCreationStatus); 
		this->m_AvailableExtensions = getAvailableExtensions();
		this->m_RequiredExtensionNames = getRequiredExtensions(debugger.isValidationEnabled());
		this->appInfo = inputApplcationInfo(); 
		createInstance(debugger);
		instanceCreated = true; 
		instanceCreationSuccessful.notify_one();
	}

	v_Instance::~v_Instance()
	{

	}

	VkApplicationInfo v_Instance::inputApplcationInfo()
	{
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Sandbox";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "WolfRenderer";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 261.1);

		return appInfo; 
	}

	void v_Instance::createInstance(v_Debugger& debugger)
	{

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		debugger.inputDebugMessengerInfo();

		createInfo.enabledExtensionCount = m_RequiredExtensionNames.size();
		createInfo.ppEnabledExtensionNames = m_RequiredExtensionNames.data();
		createInfo.pNext = &debugger.m_DebuggerCreateInfo;


		if (debugger.isValidationEnabled())
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



	std::vector<VkExtensionProperties> v_Instance::getAvailableExtensions()
	{
		//TODO: edit first nullptr when validation layers are implemented
		vkEnumerateInstanceExtensionProperties(nullptr, &m_AvailableExtensionCount, nullptr);

		std::vector<VkExtensionProperties> extensionProperties(m_AvailableExtensionCount);
		//get extension properties of ALL available extensions on our OS
		vkEnumerateInstanceExtensionProperties(nullptr, &m_AvailableExtensionCount, extensionProperties.data());

		return extensionProperties;
	}


	//stores names of Vulkan instance extensions 
	std::vector<const char*> v_Instance::getRequiredExtensions(bool isDebuggerActive)
	{
		//+1 reserved for event of debugger usage; will be modified later in optimization
		std::vector<const char*> extensionNames;

		if (isDebuggerActive)
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

	void v_Instance::destroyInstance()
	{
		vkDestroyInstance(m_Instance, nullptr);
	}
}