  #pragma once


#ifdef WLFR_PLATFORM_WINDOWS

extern WolfRenderer::Application* WolfRenderer::CreateApplication(); 

int main(int argc, char** argv)
{


	//TEMPORARY: Will be moved later
	WolfRenderer::Log::Init(); 

	WLFR_CORE_WARN("Initialized Log!");
	
	int a = 5; 

	WLFR_INFO("Hello! Var = {0}", a);

	printf("WolfRenderer Beginnings\n");
	auto app = WolfRenderer::CreateApplication(); 
	app->run();
	delete app;

	return 0; 
}
#endif