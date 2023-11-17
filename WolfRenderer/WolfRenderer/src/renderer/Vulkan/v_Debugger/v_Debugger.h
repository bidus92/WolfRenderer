#pragma once



#include "v_ValidationLayers.h"

namespace WolfRenderer
{
	//TODO: utilize debugger for multiple Vulkan resources/components 
	class WLFR_API v_Debugger : public v_ValidationLayers
	{
	
	
	public: 
		v_Debugger();

		virtual ~v_Debugger() override;

		//populates our debugMessenger struct with the needed information
		const void inputDebugMessengerInfo();

		void setupDebugger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo, VkDebugUtilsMessengerEXT* debugMessHandle);

		void destroyDebugger(VkInstance instance);


		//creates our debug extension object, implement into Vulkan class 
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
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
		static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(instance, debugMessenger, pAllocator);
			}
			
		}

	    VkDebugUtilsMessengerCreateInfoEXT getDebuggerCreateInfo() const { return debuggerCreateInfo; }
		VkDebugUtilsMessengerEXT getDebugMessenger() const { return debugMessenger; }

		VkDebugUtilsMessengerEXT* getDebugMessengerPtr()  { return &debugMessenger; }

        VkDebugUtilsMessengerCreateInfoEXT debuggerCreateInfo; 
	private:

	    VkDebugUtilsMessengerEXT debugMessenger;
		



		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
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



	};
}

