#include <Wolfrenderer.h>

//our application project inherits from wolfrenderer
class Sandbox : public WolfRenderer::Application
{

	public:

		Sandbox()
		{

		}

		~Sandbox()
		{

		}
};

WolfRenderer::Application* WolfRenderer::CreateApplication()
{
	return new Sandbox(); 
}