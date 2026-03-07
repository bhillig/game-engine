#include <Application.h>

#include <GameLayer.h>
#include <GUILayer.h>

int main(int argc, char* argv[])
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Game Engine";
	appSpec.WindowSpec.Width = 1920;
	appSpec.WindowSpec.Height = 1080;

	// Create application with specification and initialize it
	Core::Application app(appSpec);
	if (!app.Init())
	{
		LOG_CRITICAL("Failed to initialize '{}'", appSpec.Name);
		return EXIT_FAILURE;
	}

	app.AddLayer<App::GameLayer>();
	app.AddLayer<GUILayer>();

	LOG_INFO("Running '{}'", appSpec.Name);

	app.Run();

	LOG_INFO("Exiting '{}'", appSpec.Name);

    return EXIT_SUCCESS;
}
