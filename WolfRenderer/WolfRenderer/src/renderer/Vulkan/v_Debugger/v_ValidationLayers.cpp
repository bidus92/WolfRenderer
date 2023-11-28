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

	v_ValidationLayers::v_ValidationLayers()
	{

	}



	v_ValidationLayers::~v_ValidationLayers()
	{
		checkSupport();
	}
#ifdef WLFR_DEBUG

	bool v_ValidationLayers::isValidationEnabled()
	{
		if (validationLayersChecked == 0)
		{
			enableLayers = true;
			m_ValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
			validationLayersChecked++;
			return enableLayers;
		}

		else
		{
			return enableLayers;
		}

}
#else
	bool v_ValidationLayers::isValidationEnabled()
	{
		if (validationLayersChecked == 0)
		{
			enableLayers = false;
			validationLayersChecked++;
			return enableLayers;
		}
		else
		{
			return enableLayers;
		}

	}

#endif // 

	bool v_ValidationLayers::checkSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);

		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : m_ValidationLayers) {
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
