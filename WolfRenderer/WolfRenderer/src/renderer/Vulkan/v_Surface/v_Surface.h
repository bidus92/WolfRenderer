#pragma once

#include "vulkan/vulkan.h"
#include "SDL3/SDL.h"




namespace WolfRenderer
{
	class WLFR_API v_Surface
	{
	public:
		v_Surface(); 
		~v_Surface(); 
		void createWinSurface(SDL_Window* window, VkInstance instance, VkSurfaceKHR surface);
		void destroyWinSurface(VkInstance instance, VkSurfaceKHR surface);
		VkSurfaceKHR getWinSurface() const { return surface; }
	private:
		VkSurfaceKHR surface; 
	};
}