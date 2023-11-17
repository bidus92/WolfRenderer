#include "pch.h"


#include "v_Pipeline.h"


namespace WolfRenderer
{
	v_Pipeline::v_Pipeline()
	{

	}
	v_Pipeline::~v_Pipeline()
	{

	}

	void v_Pipeline::createGraphicsPipeline()
	{
		auto vertexShaderCode = shaderInterface.readFile("../v_Shaders/Shaders/spir-v/vert.spv"); 
		auto fragmentShaderCode = shaderInterface.readFile("../v_Shaders/Shaders/spir-v/frag	.spv");
	}
}