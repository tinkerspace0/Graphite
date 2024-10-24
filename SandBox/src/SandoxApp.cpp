#include <Graphite.h>

class Sandbox : public Graphite::Application
{
public:
	Sandbox() 
	{
		PushLayer(new Graphite::ImGuiLayer());
	}
	~Sandbox() {}
};


Graphite::Application* Graphite::CreateApplication()
{
	return new Sandbox();
}