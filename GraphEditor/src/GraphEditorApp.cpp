#include <Graphite.h>
#include <Graphite/Core/EntryPoint.h>

#include "Viewport/ViewportLayer.h"

namespace Graphite {

	class Graphitenut : public Application
	{
	public:
		Graphitenut()
			: Application("Graphitenut")
		{
			PushLayer(new ViewportLayer());
		}

		~Graphitenut()
		{
		}
	};

	Application* CreateApplication()
	{
		return new Graphitenut();
	}

}