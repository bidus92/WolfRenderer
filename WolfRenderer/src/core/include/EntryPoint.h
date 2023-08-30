  #pragma once

#ifdef WLFR_PLATFORM_WINDOWS


extern WolfRenderer::Application* WolfRenderer::CreateApplication(); 

int main(int argc, char** argv)
{
	printf("WolfRenderer Beginnings\n");
	auto app = WolfRenderer::CreateApplication(); 
	app->run();
	delete app;
}
#endif