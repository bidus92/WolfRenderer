#pragma once

#include "pch.h"
#include "core/core.h"
#include "events/Event.h"


//SDL_Window* window = nullptr; 

namespace WolfRenderer
{
	class WLFR_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) :
			m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		//string stream converts width height to string and prints event information,
		//when window resizing
		//DEBUG ONLY! 
		std::string ToString() const override
		{
			std::stringstream ss;

			ss << "WindowResizeEvent: " << m_Width << " , " << m_Height;
			return ss.str();
		}


		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_Width, m_Height;
	};

	class WLFR_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};

	//Tick, update, and render functions to be used as events 
	//NOTE:** these are optional to implement; As these are intrinsic to specific apps, they are always present
	class WLFR_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};

	class WLFR_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WLFR_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


}