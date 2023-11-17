#include <Wolfrenderer.h>


class ExampleLayer : public WolfRenderer::Layer
{
public: 
	ExampleLayer()
		: Layer("Example")
	{

	}

	void onUpdate() override
	{
		WLFR_INFO("ExampleLayer::Update");
	}

	void onEvent(WolfRenderer::Event& event) override
	{
		WLFR_TRACE("{0}", event);
	}

};
//our application project inherits from wolfrenderer
class Sandbox : public WolfRenderer::Application
{

	public:

		Sandbox()
		{
			//commented out for now to test out Vulkan initialization
			//pushLayer(new ExampleLayer());
		}

		~Sandbox()
		{

		}
};

WolfRenderer::Application* WolfRenderer::CreateApplication()
{
	return new Sandbox(); 
}