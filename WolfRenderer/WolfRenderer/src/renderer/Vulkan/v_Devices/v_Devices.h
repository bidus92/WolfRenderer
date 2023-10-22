#pragma once

#include "core/core.h"
#include <SDL3/SDL_vulkan.h>
#include "vulkan/vulkan.h"
#include "v_QueueFamilies/v_QueueFamilies.h"
#include "../v_Debugger/v_Debugger.h"
#include "../v_Surface/v_Surface.h"

namespace WolfRenderer
{
	class WLFR_API v_Devices
	{
		
	public:
		v_Devices(); 
		~v_Devices(); 
		void selectPhysicalDevice();
		void initialize (VkInstance instance, v_Debugger& theDebugger, VkSurfaceKHR theSurface);
		void destroyLogicalDevice();

//**Physical Device members and functions
	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; 
		uint32_t physicalDeviceCount; 
		std::vector<VkPhysicalDevice> availablePhysicalDevices;
		VkPhysicalDeviceProperties physicalDeviceProperties; 
		VkPhysicalDeviceFeatures physicalDeviceFeatures; 

		v_QueueFamilies queueFamilies; 

	private:
		std::vector<VkPhysicalDevice> locatePhysicalDevices(VkInstance instance);
		int ratePhysicalDevice(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);


//**Logical Device members and functions
	private:
		VkDevice logicalDevice = VK_NULL_HANDLE;
		VkDevice getLogicalDevice(){ return logicalDevice; }
	private:
		void createLogicalDevice(v_Debugger& theDebugger);


		
		
	};
}