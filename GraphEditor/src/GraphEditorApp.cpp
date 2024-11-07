#include <Graphite.h>
#include <Graphite/Core/EntryPoint.h>

#include "Viewport/ViewportLayer.h"

namespace Graphite {

	class GraphiteEditor : public Application
	{
	public:
		GraphiteEditor()
			: Application("GraphiteEditor")
		{
			PushLayer(new ViewportLayer());
		}

		~GraphiteEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new GraphiteEditor();
	}

}