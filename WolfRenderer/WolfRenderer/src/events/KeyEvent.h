#pragma once

#include "pch.h"
#include "events/Event.h"



namespace WolfRenderer
{
	//Events for when keys are pressed/released
	//serves as base class for key event actions 
	class WLFR_API KeyEvent : public Event
	{
		public:
			inline int GetKeycode() const const { return m_Keycode; }

			EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	
	//as constructor is protected, it cannot be made on its own but only functionality taken by
				//children classes	
	protected:
		KeyEvent(int keycode)
			: m_Keycode(keycode) {}

		int m_Keycode;

	};

	class WLFR_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		//when a key is a "repeated event" the OS sends a message saying that it was pressed, 
		//OS waits a few sceconds, and sends a bunch of "repeat events" 
		inline int GetRepeatcount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_Keycode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};


	class WLFR_API KeyReleasedEvent : public KeyEvent
	{
	public: 
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_Keycode;
			return ss.str(); 
		}

		EVENT_CLASS_TYPE(KeyReleased)


	};


}