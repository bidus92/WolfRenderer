#include "pch.h"

#include "v_QueueFamilies.h"
#include "../v_Devices.h"



namespace WolfRenderer
{

	v_QueueFamilies::v_QueueFamilies()
	{

	}
	v_QueueFamilies::~v_QueueFamilies()
	{

	}

	void v_QueueFamilies::acquire(VkPhysicalDevice device)
	{
		this->queueFamilyProperties = getQueueFamilyProperties(device);
		this->indices = findQueueFamilies();
		this->queueCreateInfo = inputQueueCreateInfo(); 
		std::cout << "Queue families acquired!\n";
	}

	void v_QueueFamilies::retrieveQueueHandle(VkDevice device, uint32_t queueIndex, VkQueue queueHandle)
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
		return indices.graphicsFamily.has_value(); 
	}

	QueueFamilyIndices v_QueueFamilies::findQueueFamilies()
	{
		for (int i = 0; i < queueFamilyProperties.size(); i++)
		{
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && !queueFamiliesFound())
			{
				indices.graphicsFamily = i; 
			}
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

	VkDeviceQueueCreateInfo v_QueueFamilies::inputQueueCreateInfo()
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
		//TODO: implement functionality to create any type of queue! (for now only utilizing graphics)
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value(); 
		//NOTE: Queue Count limited because we can implement multiple threads with command buffers
		//and submit them all at once to the main thread! 
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.f; 
		queueCreateInfo.pQueuePriorities = &queuePriority; 

		return queueCreateInfo; 

	}
}
