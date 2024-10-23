#pragma once


#ifdef GF_PLATFORM_WINDOWS
	
extern Graphite::Application* Graphite::CreateApplication();

int main(int argc, int argv)
{
	Graphite::Log::Init();
	GF_CORE_WARN("Initialized Log!");
	GF_TRACE("Initialized Log!");

	auto app = Graphite::CreateApplication();
	app->Run();
	delete app;
}

#endif