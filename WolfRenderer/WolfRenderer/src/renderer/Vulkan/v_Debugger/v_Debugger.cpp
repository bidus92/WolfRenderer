#include "pch.h"

#include "v_Debugger.h"
#include "../Vulkan.h"



namespace WolfRenderer
{

	v_Debugger::v_Debugger()
	{
		inputDebugMessengerInfo();
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

		//TODO: impelement pUserData to be utilized for multiple purposes/Vulkan classes as we abstract
		//for now it will just be the Vulkan class 
		createInfo.pUserData = nullptr;//&Vulkan::Get();

		this->debuggerCreateInfo = createInfo; 

	}

	void v_Debugger::setupDebugger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo, VkDebugUtilsMessengerEXT* debugMessHandle)
	{
		if (!enableLayers) return; 

		if (CreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, debugMessHandle) != VK_SUCCESS)
		{
			std::cout << "Failed to create Vulkan debugger\n";
		}
		else
		{
			std::cout << "Vulkan Debugger creation successful!\n";
		}
	
	}

	void v_Debugger::destroyDebugger(VkInstance instance)
	{
		if (!enableLayers) return;

		DestroyDebugUtilsMessengerEXT(instance, Vulkan::getTheDebugger(), nullptr);

		std::cout << "Vulkan Debugger successfully destroyed!\n";
	}



}