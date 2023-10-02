#pragma once
#include "pch.h"
#include "core/core.h"






//When WolfRenderer Events are currently blocking, it means
	//when an event occurs, it is dispatched immediately and 
	//dealt with as such
//Future strategies will involve having buffer events in an event and 
	//process them during the "event" part of the update stage. 
	//similar to how Vulkan does it 


namespace WolfRenderer
{
	//estabish starting enum with 0 and every sequention enum identifier will be 
	//the next num by default

	//describes the type of event
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	//utilized to filter events into different categories 
	//say a function wants to log every keyboard event or mouse event, 
	//this allows for us to select select "traits" of event types to see what they are 
	//BIT allows events to fall in multiple categories (creating a bitfield) 
	//utilizing bitshift, that value can now be activated in a different bit, allowing for a
		//string of bits to identify an event

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication		= BIT(0),
		EventCategoryInput			    = BIT(1),
		EventCategoryKeyboard           = BIT(2),
		EventCategoryMouse              = BIT(3),
		EventCategoryMouseButton        = BIT(4)
	};

	//static function that returns the name of this event 
		//(this is stati so we don't need an instance of the class controlling these events in question)  

	//GetEventType calls the GetStaticType() function
		//this is virtual so this way we can return what event type it actually is calling the 
		// GetStaticType() of the respective type 

	//GetName() returns the name in the format of a string of chars (aka a const char*) 
	//As this would be used in every event type, a macro is created for simplicity 
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; } //returns number value of the type as string of chars :) 

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	//Base class for events
	class WLFR_API Event
	{
		friend class EventDispatcher;

		public:

			//pure virtual functions (must be implemented in children classes)
			virtual EventType GetEventType() const = 0;
			virtual const char* GetName() const = 0;
			virtual int GetCategoryFlags() const = 0;

			virtual std::string ToString() const { return GetName(); }

			inline bool IsInCategory(EventCategory category)
			{
				return GetCategoryFlags() & category;
			}
			//sees if an event has been handled, this way the event doesn't transfer to other layers 
			bool m_Handled = false;


	};

	//dispatch events based on type 
	class EventDispatcher
	{
		//Creating an Event function that takes a function pointer of type bool and takes a param of any type reference
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{

		}

		//checks to see what type the event we are trying to dispatch is and if it matches
		//NOTE: no type safety to make sure type is an event	
		//if types match, the function will dispatch m_Handled bool to true for the appropriate event
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				//if the types match, it will call that function of that event
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	//utilized for our logging library to call ToString() function for our logs 
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
