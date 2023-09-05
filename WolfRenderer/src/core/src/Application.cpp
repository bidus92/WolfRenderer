#include "core/Application.h"
#include "core/Log.h"

#include "events/ApplicationEvent.h"


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
		WindowResizeEvent e(1280, 720);

		if (e.IsInCategory(EventCategoryApplication))
		{
			WLFR_TRACE(e);
		}

		if (e.IsInCategory(EventCategoryInput))
		{
			WLFR_TRACE(e);
		}
		
	
		while (true);
	}
}
