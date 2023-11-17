#pragma once
#include "../v_Shaders/v_Shaders.h"


namespace WolfRenderer
{
	class WLFR_API v_Pipeline
	{
	public: 
		v_Pipeline(); 
		~v_Pipeline(); 

		void createGraphicsPipeline(); 
	private:
		v_Shaders shaderInterface; 
	};
}