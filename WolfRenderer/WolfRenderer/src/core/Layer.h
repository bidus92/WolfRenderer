#pragma once


#include "core/core.h"
#include "events/event.h"

//Layer class to determine both draw order and event implementation 
namespace WolfRenderer
{

	class WLFR_API Layer
	{
	public: 
		Layer(const std::string& name = "Layer");
		virtual ~Layer(); 

		//when layer is attached to our app
		virtual void onAttach() {}
		//when layer is detached from our app
		virtual void onDetach() {}
        //function that occurs every frame
		virtual void onUpdate() {}
		//onEvent function to implement functionality when specific event types take place
		virtual void onEvent(Event& event) {}
		//Layer names more for debug purposes
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName; 
		//TODO: Bool to enable and disable layers to hide from layer stack
			
	};
}