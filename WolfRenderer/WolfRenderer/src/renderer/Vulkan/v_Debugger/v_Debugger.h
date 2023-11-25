#pragma once



#include "v_ValidationLayers.h"

namespace WolfRenderer
{
	//TODO: utilize debugger for multiple Vulkan resources/components 
	class WLFR_API v_Debugger : public v_ValidationLayers
	{
	
		friend class v_Instance; 
	public: 
		v_Debugger(const VkInstance& vulkanInstance, bool isInstanceCreated, std::mutex& instanceCreationStatus, std::condition_variable& instanceCreationSuccessful);

		virtual ~v_Debugger() override;

		//populates our debugMessenger struct with the needed information
		const void inputDebugMessengerInfo();

		void setupDebugger(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo, VkDebugUtilsMessengerEXT* debugMessHandle);

		void destroyDebugger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);

		

    //**Vulkan debugger object creation and destruction functions
	public:
		//creates our debug extension object, implement into Vulkan class 
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
			                                  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
			                                  VkAllocationCallbacks* pAllocator, 
			                                  VkDebugUtilsMessengerEXT* pDebugMessenger);

		//destroys vulkan debugger handle 
		static void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
			                                      VkDebugUtilsMessengerEXT debugMessenger, 
			                                      const VkAllocationCallbacks* pAllocator);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);


	//**GETTERS
	public:
	    VkDebugUtilsMessengerCreateInfoEXT getCreateInfo() const { return m_DebuggerCreateInfo; }
		VkDebugUtilsMessengerCreateInfoEXT* getCreateInfoPtr()  { return &m_DebuggerCreateInfo; }
		VkDebugUtilsMessengerEXT getDebugMessenger() const { return m_DebugMessenger; }

		VkDebugUtilsMessengerEXT* getDebugMessengerPtr()  { return &m_DebugMessenger; }

    //MEMBERS   
	private:

        VkDebugUtilsMessengerCreateInfoEXT m_DebuggerCreateInfo; 
	    VkDebugUtilsMessengerEXT m_DebugMessenger;
		




	};
}

