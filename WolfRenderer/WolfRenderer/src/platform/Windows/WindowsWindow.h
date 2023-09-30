#pragma once

#include "Window.h"

#include "SDL3/SDL.h"

namespace WolfRenderer
{
	class WindowsWindow : public Window
	{
	public: 
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

		void onUpdate() override; 

		inline unsigned int getWidth() const override { return m_Data.Width; }
		inline unsigned int getHeight() const override { return m_Data.Height; }


		//window attributes 
		inline void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown(); 
	private: 
		SDL_Window* m_Window;
		//SDL_EventFilter closeFilter;
		
		//our Event member that will poll all events for callbacks!   
		SDL_Event m_SDL_Event; 

		//Callback functions for Window events!
		void setErrorCallback(SDL_Event* sdl_event);
		void resizeWindowCallback(SDL_Event* sdl_event);
		void closeWindowCallback(SDL_Event* sdl_event);
		void setKeyCallback(SDL_Event* sdl_event);
		void setMouseButtonCallback(SDL_Event* sdl_event);
		void setMouseScrollCallback(SDL_Event* sdl_event);
		void setCursorPosCallback(SDL_Event* sdl_event);

		//TODO: Setup array of SDL events and have callback processing work about on multiple threads
		struct WindowData
		{
			std::string Title;
			unsigned int Width; 
			unsigned int Height; 

			bool vSync; 
			EventCallbackFn EventCallback; 
		};

		WindowData m_Data; 

	};
}


