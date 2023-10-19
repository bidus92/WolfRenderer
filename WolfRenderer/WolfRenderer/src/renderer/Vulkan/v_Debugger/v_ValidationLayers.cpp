#include "pch.h"

#include "v_ValidationLayers.h"

#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>
#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "core/core.h"
#include "core/LayerStack.h"

namespace WolfRenderer
{
#ifdef WLFR_DEBUG
	v_ValidationLayers::v_ValidationLayers()
	{
		this->enableLayers = true;
		this->validationLayers = { "VK_LAYER_KHRONOS_validation" };
		checkSupport();
	}
#else
	v_ValidationLayers::v_ValidationLayers()
	{
		this->enableLayers = false; 
	}
#endif

	v_ValidationLayers::~v_ValidationLayers()
	{

	}

	bool v_ValidationLayers::checkSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);

		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;	
				}
			}
			//TODO: implement message to logger system that all requested validation layers aren't supported
			if (!layerFound) {
				return false;
			}
		}

		return true;
	}





}
