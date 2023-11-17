#include "pch.h"
#include "v_Devices.h"
#include "../Vulkan.h"

#include <map>

//TODO: create arbitrary parameter for v_Devices to add functionality...
//...but also make it so parent constructor of queue families are not automatically called 
namespace WolfRenderer
{
	v_Devices::v_Devices() :
		physicalDeviceCount(0)
	{
		
	}

	void v_Devices::initialize(VkInstance instance, SDL_Window* window, v_Debugger& theDebugger, VkSurfaceKHR theSurface)
	{
		this->availablePhysicalDevices = locatePhysicalDevices(instance);
		this->selectPhysicalDevice(); 
		this->queueFamilies.acquire(this->physicalDevice, theSurface);
		this->swapChain.validateAndPopulate(this->physicalDevice, window, theSurface);
		this->createLogicalDevice(theDebugger);
		std::cout << "Physical and Logical Devices successfuly set up!\n";
		this->swapChain.createSwapChainAndImageViews(logicalDevice, theSurface, queueFamilies);
	}


	v_Devices::~v_Devices()
	{
		
	}

	std::vector<VkPhysicalDevice> v_Devices::locatePhysicalDevices(VkInstance instance)
	{
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		//TODO: implement into event system 
		if (physicalDeviceCount == 0)
		{
			throw std::runtime_error("No GPUs available on current hardware to support Vulkan!\n");
		}

		std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());

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

		for (const auto& device : availablePhysicalDevices)
		{
			int score = ratePhysicalDevice(device);
			if (isDeviceSuitable(device))
			{
               candidates.insert(std::make_pair(score, device));
			}
		}

		if (candidates.rbegin()->first > 0)
		{
			physicalDevice = candidates.rbegin()->second; 
			vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties); 
			vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
			std::cout << physicalDeviceProperties.deviceName << " has been chosen!\n";
		}


		if (physicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("Unable to find GPU for Vulkan usage\n");
		}

	}
	
	void v_Devices::createLogicalDevice(v_Debugger& theDebugger)
	{
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; 
		 
		//TODO: implement getter for queueCreateInfo struct
		createInfo.pQueueCreateInfos = this->queueFamilies.ptrToQueueCreateInfo();
		createInfo.queueCreateInfoCount = this->queueFamilies.getQueueCreateInfoSize();//static_cast<uint32_t>(this->queueFamilies.getQueueCreateInfos().size());
		createInfo.pEnabledFeatures = &this->physicalDeviceFeatures;
		createInfo.enabledExtensionCount = this->swapChain.getRequiredSwapChainExtensions().size();
		createInfo.ppEnabledExtensionNames = this->swapChain.ptrToRequiredSwapChainExtensions();
		
		if (theDebugger.isValidationEnabled())
		{
			createInfo.enabledLayerCount = theDebugger.getEnabledLayerCount();
			createInfo.ppEnabledLayerNames = theDebugger.ptrToValidationLayers(); 
		}
		else
		{
			createInfo.enabledLayerCount = 0; 
		}

		if (vkCreateDevice(this->physicalDevice, &createInfo, nullptr, &this->logicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to create Vulkan Logical Device!");
		}
		else
		{
			std::cout << "Vulkan logical device successfully created!\n";
		}

		queueFamilies.getGraphicsQueue(logicalDevice);
		queueFamilies.getPresentationQueue(logicalDevice);
	}

	void v_Devices::destroyLogicalDevice()
	{
		swapChain.destroySwapchain(logicalDevice); 
		vkDestroyDevice(logicalDevice, nullptr);
		std::cout << "Logical device successfully destroyed!\n";
	}
	
}
