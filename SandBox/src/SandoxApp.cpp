#include <Graphite.h>

class Sandbox : public Graphite::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};


Graphite::Application* Graphite::CreateApplication()
{
	return new Sandbox();
}