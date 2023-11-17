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

	//TODO: BREAK UP PIPELINE OBJECT SETUP INTO SEPARATE FUNCTIONS! 
	void v_Pipeline::createGraphicsPipeline(const VkDevice& logicalDevice, v_SwapChain& swapchain)
	{

	    theShaders.createShaderModules(logicalDevice); 
		createRenderPass(logicalDevice, swapchain); 
		

		//establishes our dynamic components of our graphics pipeline
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();


		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO; 
		vertexInputInfo.vertexBindingDescriptionCount = 0; 
		vertexInputInfo.pVertexBindingDescriptions = nullptr; //null for now ;
		vertexInputInfo.vertexAttributeDescriptionCount = 0; 
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; 

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO; 
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE; 


		viewport.x = 0.f; 
		viewport.y = 0.f;
		viewport.width = static_cast<float>(swapchain.getImageExtentWidth());
		viewport.height = static_cast<float>(swapchain.getImageExtentHeight());
		viewport.minDepth = 0.f; 
		viewport.maxDepth = 1.f; 

		scissor.offset = { 0, 0 }; 
		scissor.extent = swapchain.getSwapChainImageExtent(); 

		//our viewport and scissor info for the graphicsPipeline to set up
//NOTE: possible to setup multiple viewports and scissors in a single command buffer with dynamic states	
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport; 
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor; 

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO; 
		rasterizer.depthClampEnable = VK_FALSE; //good for shadow mapping - implementation to be used later
		rasterizer.rasterizerDiscardEnable = VK_FALSE; 
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL; //utilize for rendering modes: Wireframe, full, points
		rasterizer.lineWidth = 1.f; 
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; 
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; 

		//depthBias params useful for shadowmapping - revisit later; 
		rasterizer.depthBiasEnable = VK_FALSE; 
		rasterizer.depthBiasConstantFactor = 0.f; 
		rasterizer.depthBiasClamp = 0.f; 
		rasterizer.depthBiasSlopeFactor = 0.f; 

		//will be incorporated later for antialiasing purposes
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO; 
		multisampling.sampleShadingEnable = VK_FALSE; 
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; 
		multisampling.minSampleShading = 1.f; 
		multisampling.pSampleMask = nullptr; 
		multisampling.alphaToCoverageEnable = VK_FALSE; 
		multisampling.alphaToOneEnable = VK_FALSE; 

		//will be incorporated later
		//VkPipelineDepthStencilStateCreateInfo depthAndStencil{};
			

		//Blending will also be fully incorporated later
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | 
			VK_COLOR_COMPONENT_G_BIT | 
			VK_COLOR_COMPONENT_B_BIT | 
			VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE; 
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; 
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; 
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; 
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; 
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; 
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; 

		//For Bitwise operations
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE; 
		colorBlending.logicOp = VK_LOGIC_OP_COPY; 
		colorBlending.attachmentCount = 1; 
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.f; 
		colorBlending.blendConstants[1] = 0.f;
		colorBlending.blendConstants[2] = 0.f;
		colorBlending.blendConstants[3] = 0.f;

		//empty for now, will evolve later; specify push constants here
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO; 
		pipelineLayoutCreateInfo.setLayoutCount = 0; 
		pipelineLayoutCreateInfo.pSetLayouts = nullptr; 
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0; 
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &graphicsPipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to create Vulkan graphics pipeline layout!\n");
		}
		std::cout << "Vulkan Pipeline Layout creation successful!\n";
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO; 
		pipelineInfo.stageCount = theShaders.getNumShaderStages(); //shader
		pipelineInfo.pStages = theShaders.shaderStages(); 
		pipelineInfo.pVertexInputState = &vertexInputInfo; 
		pipelineInfo.pInputAssemblyState = &inputAssembly; 
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer; 
		pipelineInfo.pMultisampleState = &multisampling; 
		pipelineInfo.pDepthStencilState = nullptr; 
		pipelineInfo.pColorBlendState = &colorBlending; 
		pipelineInfo.pDynamicState = &dynamicStateCreateInfo;
		pipelineInfo.layout = graphicsPipelineLayout; 
		pipelineInfo.renderPass = renderPass; //allows for multiple renderpass usage for graphics pipeline though they have to be compatible
		pipelineInfo.subpass = 0; //index of subpass where this graphics pipeline will be used

		//parameters to create a graphics pipeline based on an existing one
		//specify VK_PIPELINE_CREATE_DERIVATIVE_BIT in the pipelineCreateInfo flags field if you were to use
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; 
		pipelineInfo.basePipelineIndex = -1; 

		//designed to create multiple pipelines in one call! 
		//TODO: In future, utilize VkPipelineCache (currently VK_NULL_HANDLE) to optimize creation when multiple calls are to be needed
		if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan Graphics Pipeline\n");
		}
		else
		{
			std::cout << "Vulkan Graphics Pipeline creation successful!\n";
		}

		theShaders.destroyShaderModules(logicalDevice); 

	}

	//TODO: add more customiazability for render pass creation for different types of pipelnes (i.e. Graphics, compute, etc.) 
	void v_Pipeline::createRenderPass(const VkDevice& logicalDevice, v_SwapChain& swapchain)
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapchain.getImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; 
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; 
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; 
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; 
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; 
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //don't care the format of image layout on initialization
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //format to be optimal for presenting to the swapchain when render pass completes

		//attachment reference for subpass usage 
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0; //attachment index
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; //we want our attachment to be optimized as a color attachment

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //type of subpass for graphics pipeline specfically
		subpass.colorAttachmentCount = 1; 
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0; 
		
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; 
		dependency.srcAccessMask = 0; 
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; 
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; 



		VkRenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO; 
		renderPassCreateInfo.attachmentCount = 1; 
		renderPassCreateInfo.pAttachments = &colorAttachment; 
		renderPassCreateInfo.subpassCount = 1; 
		renderPassCreateInfo.pSubpasses = &subpass;
		renderPassCreateInfo.dependencyCount = 1; 
		renderPassCreateInfo.pDependencies = &dependency; 

		if (vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &this->renderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan Render Pass!\n");
		}

		std::cout << "Vulkan Render Pass creation successful!\n";
	}

	VkRenderPass v_Pipeline::recreateRenderPass(const VkDevice& logicalDevice, v_SwapChain& swapchain, VkRenderPass& theRenderPass)
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapchain.getImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //don't care the format of image layout on initialization
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //format to be optimal for presenting to the swapchain when render pass completes

		//attachment reference for subpass usage 
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0; //attachment index
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; //we want our attachment to be optimized as a color attachment

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //type of subpass for graphics pipeline specfically
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;



		VkRenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = 1;
		renderPassCreateInfo.pAttachments = &colorAttachment;
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpass;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &theRenderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan Render Pass!\n");
		}
		
		std::cout << "Vulkan Render Pass recreation successful!\n";

		return theRenderPass; 
	}

	void v_Pipeline::destroyPipeline(const VkDevice& logicalDevice)
	{
		vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(logicalDevice, graphicsPipelineLayout, nullptr);
		vkDestroyRenderPass(logicalDevice, renderPass, nullptr); 

		std::cout << "Vulkan Pipeline successfully destroyed!\n";
	}
}