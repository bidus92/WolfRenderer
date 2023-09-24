
#include "pch.h"

#include "core/Application.h"
#include "core/Log.h"

#include "events/ApplicationEvent.h"

#include <SDL3/SDL.h>

namespace WolfRenderer
{

	Application::Application()
	{

	};	


	Application::~Application() 
	{

	};

	void Application::run()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		WindowResizeEvent e(1280, 720);

		//should return appropriate log
		if (e.IsInCategory(EventCategoryApplication))
		{
			WLFR_TRACE(e);
		}

		//should not return anything
		if (e.IsInCategory(EventCategoryInput))
		{
			WLFR_TRACE(e);
		}
		
	
		while (true);
	}
}
