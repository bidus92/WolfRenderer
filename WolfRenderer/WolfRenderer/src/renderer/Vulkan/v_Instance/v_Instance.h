#pragma once



#include <vulkan/vulkan.h>
#include "../v_Debugger/v_Debugger.h"
#include <condition_variable>



namespace WolfRenderer
{
	class WLFR_API v_Instance
	{

	public:
		v_Instance(v_Debugger& debugger, std::mutex& instanceCreationStatus,  std::condition_variable& instanceCreationSuccessful); 
		~v_Instance();

		VkInstance getInstance() const { return m_Instance; }

		void destroyInstance();

		//for sync with debugger
		bool isInstanceCreated() const { return instanceCreated; }
	private:
		void createInstance(v_Debugger& debugger);

	private:
		VkInstance m_Instance;
		VkApplicationInfo appInfo{};
		bool instanceCreated = false; 
		std::vector<VkExtensionProperties> m_AvailableExtensions;
        //array of available extensions on our OS
		std::vector<const char*> m_RequiredExtensionNames;

		uint32_t m_AvailableExtensionCount;
        //number of required extensions for Vulkan to function 
		uint32_t m_RequiredExtensionCount;


	private:
		VkApplicationInfo inputApplcationInfo();
		std::vector<VkExtensionProperties> getAvailableExtensions();
		//populates our extension count and names
		std::vector<const char*> getRequiredExtensions(bool isDebuggerActive);

	};
}