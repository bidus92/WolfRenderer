#pragma once
#include <vulkan/vulkan.h>


#include "v_Instance/v_Instance.h"
#include "v_Debugger/v_Debugger.h"
#include "v_Devices/v_Devices.h"
#include "v_Surface/v_Surface.h"



#include "events/ApplicationEvent.h"
#include "core/core.h"
#include "core/LayerStack.h"

#include <mutex>
#include <condition_variable>
//Utilization of Singleton for our Vulkan Renderer
namespace WolfRenderer
{
	class WLFR_API Vulkan
	{
	public:

		static Vulkan& Get();

		//deletion of assignment operator to prevent singleton from being utilized by any other variable
		Vulkan operator=(const Vulkan& other) = delete;

		//public function to initialize our instance 
		static void createVulkan(SDL_Window* window);

		//public function to destroy our Vulkan instance
		static void closeVulkan();

		static void drawFrame();

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
		void draw();

    //synchronization variables
	private:
		std::mutex instanceCreationStatus; 
		std::condition_variable instanceCreationSuccessful; 

	private:

		v_Instance m_Instance{ m_Debugger, instanceCreationStatus, instanceCreationSuccessful };
		v_Debugger m_Debugger{ m_Instance.getInstance(), m_Instance.isInstanceCreated(), instanceCreationStatus, instanceCreationSuccessful };
		v_Devices m_Devices;
		v_Surface m_WinSurface;
	};
}
