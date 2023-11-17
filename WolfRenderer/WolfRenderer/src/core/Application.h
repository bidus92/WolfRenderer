#pragma once

#include "core/core.h"

#include "Window.h"
#include "core/LayerStack.h"
#include "events/Event.h"
#include "events/ApplicationEvent.h"



namespace WolfRenderer
{

	class WLFR_API Application
	{
	public:
		Application();

		virtual ~Application(); 

		void run(); 

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		
	private:
		//event functions must be bools to indicate action be taken by the Event Dispatcher
		bool onWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack; 
	};	
		//TO BE DEFINED IN CLIENT (SANDBOX APP) 
		Application* CreateApplication();

}


