#pragma once

#ifdef GF_PLATFORM_WINDOWS

extern Graphite::Application* Graphite::CreateApplication();

int main(int argc, char** argv)
{
	Graphite::Log::Init();
	GF_CORE_WARN("Initialized Log!");
	int a = 5;
	GF_INFO("Hello! Var={0}", a);

	auto app = Graphite::CreateApplication();
	app->Run();
	delete app;
}

#endif