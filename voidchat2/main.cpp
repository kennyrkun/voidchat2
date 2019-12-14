#include "VoidChat.hpp"

#include "AppEngine.hpp"
#include "InitialiseState.hpp"

int main(int argc, char* argv[])
{
	AppSettings settings;
	settings.launchOptions = { argc, argv };

	AppEngine engine;
	engine.Init(settings);

	engine.PushState(new InitialiseState);

	while (engine.isRunning())
	{
		engine.HandleEvents();
		engine.Update();
		engine.Draw();
	}

	return 0;
}
