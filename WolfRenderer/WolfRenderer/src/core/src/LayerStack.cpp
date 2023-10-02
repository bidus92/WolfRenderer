#include "pch.h"
#include "core/LayerStack.h"


namespace WolfRenderer
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin(); 
	}

	//For now: layers all will exist though if we change levels, a new layer stack would be made
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer; 
		}
	}

	//pushLayer pushes layers to the first half of the layer list for render order purposes
	void LayerStack::pushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}
	//overlays get pushed to the second half of the layer list so that they are renderered last
	void LayerStack::pushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--; 
		}
	}
}