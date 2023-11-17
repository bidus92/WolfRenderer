
#include "pch.h"

#include "core/Application.h"
#include "core/Log.h"
#include "vulkan/vulkan.h"
#include <SDL3/SDL.h>

namespace WolfRenderer
{
    #define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->setEventCallback(BIND_EVENT_FN(Application::onEvent));
		//TODO: implement function in Window.h for Vulkan access to debugger! 
	};	


	Application::~Application() 
	{

	};

	void Application::pushLayer(Layer* layer)
	{
		m_LayerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
		m_LayerStack.pushOverlay(overlay);
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		//TODO: potential allow for asynchronous dispatching of events for various functions and actions
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClosed));

		WLFR_CORE_TRACE("{0}", e);

		//go backward through the layer stack and call onEvent() to start with layer closest to surface
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			//we break if the event is handled as to ensure that events below this do not receive the same event notification
			(*--it)->onEvent(e);
			if (e.m_Handled)
				break; 
		}
	}


	void Application::run()
	{	
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
			{
				layer->onUpdate(); 
			}
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
