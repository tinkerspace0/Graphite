#pragma once
#include "Graphite/Core/Base.h"

#ifdef GF_PLATFORM_WINDOWS

extern Graphite::Application* Graphite::CreateApplication();

int main(int argc, char** argv)
{
	Graphite::Log::Init();

	GF_PROFILE_BEGIN_SESSION("Startup", "GraphiteProfile-Startup.json");
	auto app = Graphite::CreateApplication();
	GF_PROFILE_END_SESSION();

	GF_PROFILE_BEGIN_SESSION("Runtime", "GraphiteProfile-Runtime.json");
	app->Run();
	GF_PROFILE_END_SESSION();

	GF_PROFILE_BEGIN_SESSION("Shutdown", "GraphiteProfile-Shutdown.json");
	delete app;
	GF_PROFILE_END_SESSION();
}

#endif