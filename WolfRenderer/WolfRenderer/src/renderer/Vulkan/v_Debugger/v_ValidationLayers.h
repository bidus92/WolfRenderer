#pragma once

#include "pch.h"

#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>
#include "events/Event.h" //TODO- encorporate validation layer messages to logging system
#include "events/ApplicationEvent.h" //TODO: encorporate Validation Layer messages to logging system
#include "core/core.h"
#include "core/LayerStack.h"


namespace WolfRenderer
{

	//"v" abbreviated for vulkan": holds our validation layer data 
	class WLFR_API v_ValidationLayers
	{

	public:	

		uint32_t getEnabledLayerCount() { return m_ValidationLayers.size(); }
		const char** ptrToValidationLayers() { return m_ValidationLayers.data(); }

		bool isValidationEnabled();
		bool checkSupport();

	protected:
		v_ValidationLayers();
		virtual ~v_ValidationLayers(); 

	private:

		// TODO: add event dispatcher for Vulkan Events! Validation Layers, notifications, etc. 
		// TODO: cleanup struct establishing functions
		

	private:
		bool enableLayers;
		std::vector<const char*> m_ValidationLayers;
		int validationLayersChecked = 0; 

	};
}
