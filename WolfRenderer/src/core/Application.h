#pragma once

#include "core/core.h"
#include "events/event.h"

namespace WolfRenderer
{

	class WLFR_API Application
	{
	public:
		Application();

		virtual ~Application(); 

		void run(); 

	};	
		//TO BE DEFINED IN CLIENT (SANDBOX APP) 
		Application* CreateApplication();

}


