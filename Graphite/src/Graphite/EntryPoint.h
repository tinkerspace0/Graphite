#pragma once


#ifdef GF_PLATFORM_WINDOWS
	
extern Graphite::Application* Graphite::CreateApplication();

int main(int argc, int argv)
{
	auto app = Graphite::CreateApplication();
	app->Run();
	delete app;
}

#endif