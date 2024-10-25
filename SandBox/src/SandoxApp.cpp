#include <Graphite.h>
#include "imgui.h"

class ConsoleWindow : public ImGuiWindow
{
public:
	void Render() override
	{
		// Create the Console window
		ImGui::Begin("Console");
		ImGui::Text("Console Output");
		ImGui::Separator();
		ImGui::Text("Log message 1...");
		ImGui::Text("Log message 2...");
		ImGui::End();

		// Create the Settings window
		ImGui::Begin("Settings");
		ImGui::Text("Settings Panel");
		ImGui::Separator();
		ImGui::End();

		// Show a sample window for testing
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}
};

class Sandbox : public Graphite::Application
{
public:
	Sandbox() 
	{
		Graphite::ImGuiLayer* MyImGuiLayer = new Graphite::ImGuiLayer();
		PushLayer(MyImGuiLayer);
		auto MyWindow = ConsoleWindow();
		MyImGuiLayer->AddImGuiWindow(&MyWindow);
	}
	~Sandbox() {}
};


Graphite::Application* Graphite::CreateApplication()
{
	return new Sandbox();
}