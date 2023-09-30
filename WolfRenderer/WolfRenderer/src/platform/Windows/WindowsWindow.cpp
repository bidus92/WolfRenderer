#include "pch.h"
#include "WindowsWindow.h"
#include "core/core.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_thread.h"
namespace WolfRenderer
{
	static bool s_SDLInitialized = false; 

	void WindowsWindow::setErrorCallback(SDL_Event* sdl_event)
	{
		SDL_ClearError();

		std::string error = SDL_GetError();  //SDL_GetErrBuf();
		if (error.size() > 1)
		{
			WLFR_CORE_ERROR(SDL_GetError());
		}
	}

	//callback function for SDL for resizing Windows
	void WindowsWindow::resizeWindowCallback(SDL_Event* sdl_event)
	{
			//temps to store new window width and height 
			int width; 
			int height; 

			//stores window width and height in pixels in buffer vars
			SDL_GetWindowSizeInPixels(m_Window, &width, &height);

			//gets pointer to our m_Data holding our window data casting function to a function pointer
			WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "Window Props");
			data.Width = width; 
			data.Height = height;  
			
			WindowResizeEvent event(width, height);
	        

			data.EventCallback(event);
	}

	//callback function for SDL for closing the window
	void WindowsWindow::closeWindowCallback(SDL_Event* sdl_event)
	{
			//gets pointer to our m_Data holding our window data casting function to a function pointer
			WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "Window Props");
			WindowCloseEvent event; 
			data.EventCallback(event);
			//SDL_DestroyWindow(m_Window);
	}

	
	//callback function for SDL for identifying key presses, holds, and releases
	void WindowsWindow::setKeyCallback(SDL_Event* sdl_event)
	{
		WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "Window Props");
		static int repeatCount = 1;
		int action = sdl_event->key.type;
		switch (action)
		{
		    case SDL_EVENT_KEY_DOWN:
				//first checks to see if the key has been held down for a repeat count otherwise we input the repeat count as 0 
				if (sdl_event->key.repeat > 0)
				{
					KeyPressedEvent event(sdl_event->key.keysym.sym, repeatCount);
					data.EventCallback(event);
				    repeatCount += 1; 
				}
				else 
				{
                    KeyPressedEvent event(sdl_event->key.keysym.sym, 0);
					data.EventCallback(event);
				}
				break; 
			case SDL_EVENT_KEY_UP:
				KeyPressedEvent event(sdl_event->key.keysym.sym, repeatCount);
				data.EventCallback(event);
				repeatCount = 0;
				break;	
		}

	}

	void WindowsWindow::setMouseButtonCallback(SDL_Event* sdl_event)
	{
		WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "Window Props");

		int action = sdl_event->button.state;
		MouseButtonPressedEvent p_event(sdl_event->button.button);
		MouseButtonReleasedEvent r_event(sdl_event->button.button);

		switch (action)
		{
		case SDL_PRESSED:
			data.EventCallback(p_event);
			break;
		case SDL_RELEASED:
			data.EventCallback(r_event);
			break;
		}
	}

	//NOTE: May need change for input params for event
	void WindowsWindow::setMouseScrollCallback(SDL_Event* sdl_event)
	{
		WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "Window Props");

		MouseScrolledEvent event(sdl_event->wheel.x, sdl_event->wheel.y);

		data.EventCallback(event);

	}

	void WindowsWindow::setCursorPosCallback(SDL_Event* sdl_event)
	{
		WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "Window Props");

		MouseMovedEvent event(sdl_event->motion.x, sdl_event->motion.y);

		data.EventCallback(event);

	}
	// 
	//overloads the function with appropriate type 
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title; 
		m_Data.Width = props.Width; 
		m_Data.Height = props.Height; 

		WLFR_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_SDLInitialized)
		{
			//returns 0 upon success
			int success = SDL_Init(SDL_INIT_EVERYTHING);

			//NOTE: bool true values return an int of 1, hence the assertion of "success + 1"  
			WLFR_CORE_ASSERT(success + 1, "Could not initialize SDL3!");
		    
			setErrorCallback(&m_SDL_Event); 

			s_SDLInitialized = true; 
		}
		//TODO: set last parameter of SDL_CreateWindow to work with Vulkan interfaces...more on that later
		m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, SDL_WINDOW_RESIZABLE);
		
		SDL_SetWindowData(m_Window, "Window Props", &m_Data); 
		setVSync(true);
	}

	//Set SDL event callbacks!


	void WindowsWindow::Shutdown()
	{
		SDL_DestroyWindow(m_Window);
	}

	void WindowsWindow::onUpdate()
	{ 
		//Possible TODO: Make event a varible in the Window.h file for all platforms to inherit
		while (SDL_PollEvent(&m_SDL_Event))
		{
			//gets pointer to our m_Data holding our window data casting function to a function pointer
			//WindowData& data = *(WindowData*)SDL_GetWindowData(m_Window, "Window Props");
			switch (m_SDL_Event.type)
			{
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				resizeWindowCallback(&m_SDL_Event);
				break;
			case SDL_EVENT_QUIT:
				closeWindowCallback(&m_SDL_Event);
				break;
			case SDL_EVENT_KEY_DOWN:
				setKeyCallback(&m_SDL_Event);
				break;
			case SDL_EVENT_KEY_UP: 
				setKeyCallback(&m_SDL_Event);
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				setMouseButtonCallback(&m_SDL_Event);
				break;
			case  SDL_EVENT_MOUSE_BUTTON_DOWN:
				setMouseButtonCallback(&m_SDL_Event);
				break;
			case SDL_EVENT_MOUSE_WHEEL:
				setMouseScrollCallback(&m_SDL_Event);
				break;
			case SDL_EVENT_MOUSE_MOTION:
				setCursorPosCallback(&m_SDL_Event);
				break;
			}
			//SDL_WaitEvent(&another_event);
		};

		//TODO: implement vulkan swapping of buffers. Will be utilized later
	}

	void WindowsWindow::setVSync(bool enabled)
	{
	    //TODO: implement in Vulkan setup 
		/*
		* if(enabled)
		*     swapinterval is 1
		* else 
		*     swapinterval is 0
		* 
		* 
		* m_Data.vSync = enabled; 
		*/
	}

	bool WindowsWindow::isVSync() const 
	{
		return m_Data.vSync; 
	}
}