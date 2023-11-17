#pragma once


#include "core/core.h"
#include "core/Layer.h"

#include <vector>


namespace WolfRenderer
{
	class WLFR_API LayerStack
	{
	public: 
		LayerStack();
		~LayerStack(); 

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		//iterator indicating a begin and end to use a RBFor loop in Application
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		//holds our layers
		std::vector<Layer*> m_Layers;
		//pointer to memory allocated for our number of Layers 
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}