#include "pch.h"

#include "v_Surface.h"
#include "SDL3/SDL_vulkan.h"




namespace WolfRenderer
{
	v_Surface::v_Surface()
	{

	}
	v_Surface::~v_Surface()
	{

	}

	void v_Surface::createWinSurface(SDL_Window* window, VkInstance instance, VkSurfaceKHR surface)
	{
		if (SDL_Vulkan_CreateSurface(window, instance, &this->surface) != SDL_TRUE)
		{
			throw std::runtime_error("Failure to create Vulkan Window Surface");
		}
		else
		{
			std::cout << "Vulkan window surface creation successful!\n";
		}
	}

	void v_Surface::destroyWinSurface(VkInstance instance, VkSurfaceKHR surface)
	{
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

}