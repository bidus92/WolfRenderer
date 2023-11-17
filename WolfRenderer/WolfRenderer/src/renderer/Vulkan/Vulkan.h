#pragma once
#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>

#include "v_Debugger/v_Debugger.h"
#include "v_Devices/v_Devices.h"
#include "v_Surface/v_Surface.h"

#include "events/ApplicationEvent.h"
#include "core/core.h"
#include "core/LayerStack.h"

//Utilization of Singleton for our Vulkan Renderer
namespace WolfRenderer
{
	class WLFR_API Vulkan
	{
		friend class WindowsWindow;
		public:

			static Vulkan& Get();

			//deletion of assignment operator to prevent singleton from being utilized by any other variable
			Vulkan operator=(const Vulkan& other) = delete;

			//public function to initialize our instance 
			static void createVulkan(SDL_Window* window);

			//public function to destroy our Vulkan instance
			static void closeVulkan();

			static VkInstance getInstance();

		    static VkDebugUtilsMessengerEXT getTheDebugger();


		private:
			Vulkan();

			virtual ~Vulkan();

			Vulkan(const Vulkan&) = delete;
			Vulkan(Vulkan&&) = delete; 
			Vulkan& operator=(Vulkan&) = delete;
			Vulkan& operator=(Vulkan&&) = delete;
			//initializes vulkan
			void init_Vulkan(SDL_Window* window);

			//destroys the Vulkan instance
			void destroy_Vulkan();

	

			std::vector<VkExtensionProperties> getAvailableExtensions(); 


			//populates our extension count and names
			std::vector<const char*> getRequiredExtensions();

			VkInstance retrieveTheInstance() const; 


		private:
			//handle for our Vulkan instance
			VkInstance m_Instance;

			uint32_t m_AvailableExtensionCount; 
			
			std::vector<VkExtensionProperties> m_AvailableExtensions; 

			//number of required extensions for Vulkan to function 
			uint32_t m_RequiredExtensionCount;

			//array of available extensions on our OS
			std::vector<const char*> m_RequiredExtensionNames;

			
		private:
			v_Debugger debugger;
			v_Devices devices;
			v_Surface winSurface; 

	};
}
