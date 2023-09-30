#pragma once

#include "core/core.h"
#include "events/event.h"
#include "events/ApplicationEvent.h"
#include "Window.h"

namespace WolfRenderer
{

	class WLFR_API Application
	{
	public:
		Application();

		virtual ~Application(); 

		void run(); 

		void onEvent(Event& e);
		
	private:
		bool onWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};	
		//TO BE DEFINED IN CLIENT (SANDBOX APP) 
		Application* CreateApplication();

}


