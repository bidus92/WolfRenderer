#include "pch.h"

#include "v_QueueFamilies.h"
#include <set>


#include "../v_Devices.h"



namespace WolfRenderer
{

	v_QueueFamilies::v_QueueFamilies()
	{

	}
	v_QueueFamilies::~v_QueueFamilies()
	{

	}

	void v_QueueFamilies::acquire(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		this->queueFamilyProperties = getQueueFamilyProperties(device);
		this->indices = findQueueFamilies(device, surface);
		this->queueCreateInfos = inputQueueCreateInfos(); 
		std::cout << "Queue families acquired!\n";
	}

	void v_QueueFamilies::retrieveQueueHandle(const VkDevice& device, const uint32_t& queueIndex, VkQueue queueHandle)
	{
		vkGetDeviceQueue(device, queueIndex, 0, &queueHandle);
	}

	std::vector<VkQueueFamilyProperties> v_QueueFamilies::getQueueFamilyProperties(VkPhysicalDevice device)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);


		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		return queueFamilies; 
	}

	bool v_QueueFamilies::queueFamiliesFound()
	{
		return indices.graphicsFamily.has_value() && indices.presentFamily.has_value(); 
	}

	QueueFamilyIndices v_QueueFamilies::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		for (int i = 0; i < queueFamilyProperties.size(); i++)
		{
			//Find Graphics Queue Family Index
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i; 
			}

			//Find Presentation Queue Family Index
			VkBool32 presentSupport = false; 
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			
			if (presentSupport)
			{
				indices.presentFamily = i; 
			}

			//break the loop if all are found
			if (queueFamiliesFound())
			{
				break; 
			}
		}

		if (!queueFamiliesFound())
		{
			throw std::runtime_error("Unable to find required queue families on GPU!\n");
		}

		return indices; 
	}	

	std::vector<VkDeviceQueueCreateInfo> v_QueueFamilies::inputQueueCreateInfos()
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo; 

			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
			//TODO: implement functionality to create any type of queue! (for now only utilizing graphics)
			queueCreateInfo.queueFamilyIndex = queueFamily; 
			//NOTE: Queue Count limited because we can implement multiple threads with command buffers
			//and submit them all at once to the main thread! 
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.flags = 0; 
			queueCreateInfo.pNext = 0; 
			queueCreateInfo.pQueuePriorities = &this->queuePriority; 

			queueCreateInfos.push_back(queueCreateInfo);
		}

		return queueCreateInfos; 

	}

	void v_QueueFamilies::getGraphicsQueue(VkDevice logicalDevice)
	{
		vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
	}
	void v_QueueFamilies::getPresentationQueue(VkDevice logicalDevice)
	{
		vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &this->presentQueue);
	}
}
