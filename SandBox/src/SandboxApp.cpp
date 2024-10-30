#include <Graphite.h>
#include <Graphite/Core/EntryPoint.h>

#include "ExampleLayer.h"

class Sandbox : public Graphite::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Graphite::ViewportLayer());
	}

	~Sandbox()
	{
	}
};

Graphite::Application* Graphite::CreateApplication()
{
	return new Sandbox();
}