#pragma once

#include "core.h"

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


