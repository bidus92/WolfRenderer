#include "pch.h"

#include "v_Debugger.h"
#include "../Vulkan.h"
#include <condition_variable>


namespace WolfRenderer
{

	v_Debugger::v_Debugger(const VkInstance& vulkanInstance, bool isInstanceCreated, std::mutex& instanceCreationStatus, std::condition_variable& instanceCreationSuccessful)
	{
		std::unique_lock<std::mutex> beginDebuggerCreation(instanceCreationStatus);
		instanceCreationSuccessful.wait(beginDebuggerCreation, [&] {return isInstanceCreated ? true : false; });
		if (isValidationEnabled())
		{
			std::cout << "Validation Layers Enabled!\n";
			setupDebugger(vulkanInstance, m_DebuggerCreateInfo, &m_DebugMessenger);
		}
		else
		{
			std::cout << "Validation Layers Disabled!\n";
		}

	}


	v_Debugger::~v_Debugger()
	{

	}

	const void v_Debugger::inputDebugMessengerInfo()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; 
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
			                     VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
			                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback; 

		
		createInfo.pUserData = nullptr;//&Vulkan::Get();

		m_DebuggerCreateInfo = createInfo; 

	}

	void v_Debugger::setupDebugger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo, VkDebugUtilsMessengerEXT* debugMessHandle)
	{
		if (!isValidationEnabled()) return; 

		if (CreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, debugMessHandle) != VK_SUCCESS)
		{
			std::cout << "Failed to create Vulkan debugger\n";
		}
		else
		{
			std::cout << "Vulkan Debugger creation successful!\n";
		}
	
	}

	void v_Debugger::destroyDebugger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger)
	{
		if (!isValidationEnabled()) return;

		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

		std::cout << "Vulkan Debugger successfully destroyed!\n";
	}

	//creates our debug extension object, implement into Vulkan class 
	VkResult v_Debugger::CreateDebugUtilsMessengerEXT(VkInstance instance, 
		                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
		                                              VkAllocationCallbacks* pAllocator, 
		                                              VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}

	}

	//destroys vulkan debugger handle 
	void v_Debugger::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}

	}

	VKAPI_ATTR VkBool32 VKAPI_CALL v_Debugger::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
		{

			//TODO: implement message into event system 
			if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			{
				std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
			}

			return VK_FALSE;
		}


}