#pragma once


#include "pch.h"

#include "core/core.h"
#include "events/Event.h"




namespace WolfRenderer
{
	struct WindowProps
	{
		std::string Title; 
		unsigned int Width; 
		unsigned int Height; 

		WindowProps(const std::string& title = "WolfRenderer Engine",
			unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};


	//Pure virtual interface class for platform window definitions to work with
	class WLFR_API Window
	{
	public: 
		using EventCallbackFn = std::function<void(Event&)>; 

		virtual ~Window() {}; 

		virtual void onUpdate() = 0; 

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;


		virtual void setEventCallback(const EventCallbackFn& callback) = 0; 
		virtual void setVSync(bool enabled) = 0; 
		virtual bool isVSync() const = 0; 

		//creates our window with respective window properties with default constructor settings
		//calling create in any child class of Window will result in the default properties we have established for the window 
	         //to be made
		static Window* Create(const WindowProps& props = WindowProps());

	};


}