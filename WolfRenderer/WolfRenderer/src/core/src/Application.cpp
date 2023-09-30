
#include "pch.h"

#include "core/Application.h"
#include "core/Log.h"


#include <SDL3/SDL.h>

namespace WolfRenderer
{
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->setEventCallback(BIND_EVENT_FN(onEvent));
	};	


	Application::~Application() 
	{

	};

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClosed));

		WLFR_CORE_TRACE("{0}", e);
	}


	void Application::run()
	{	
		while (m_Running)
		{
			m_Window->onUpdate();
		}

	}	
	
	//takes in WindowCloseEvent via event dispatcher and closes Run loop 
	bool Application::onWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false; 
		return true;
	}

}
