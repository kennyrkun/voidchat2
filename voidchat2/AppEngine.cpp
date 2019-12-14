#include "AppEngine.hpp"
#include "AppState.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem;

// INFO: keep this bare minimums
// AppEngine is the core of the application,
// and should rely on as little external
// resources as reasonably possible.

void AppEngine::Init(AppSettings settings_)
{
	std::cout << "AppEngine Init" << std::endl;

	settings = settings_;

	for (size_t i = 0; i < settings.launchOptions.argc; i++)
	{
		std::cout << i << ": " << settings.launchOptions.argv[i] << std::endl;

		if (std::string(settings.launchOptions.argv[i]) == "-serverIp")
		{
			settings.server.serverIpAddress = settings.launchOptions.argv[i + 1];
			std::cout << "set server ip to " << settings.server.serverIpAddress << std::endl;
		}

		if (std::string(settings.launchOptions.argv[i]) == "-serverPort")
		{
			settings.server.serverPort = std::stoi(settings.launchOptions.argv[i + 1]);
			std::cout << "set server ip to " << settings.server.serverIpAddress << std::endl;
		}
	}

	for (int i = 0; i < settings.launchOptions.argc; i++)
	{
		if (std::string(settings.launchOptions.argv[i]) == "-vsync")
			settings.vsync = true;

		if (std::string(settings.launchOptions.argv[i]) == "-maxfps")
			// next arg should be an int (the maxfps)
			settings.maxfps = std::stoi(settings.launchOptions.argv[i += 1]);
	}

	if (!fs::exists("./sflcars"))
		fs::create_directory("./sflcars");

	window.create(sf::VideoMode(500, 400), "VoidChat 2", sf::Style::Default);

	running = true;

	std::cout << "AppEngine ready" << std::endl;
}

void AppEngine::Cleanup()
{
	std::cout << "Cleaning up AppEngine." << std::endl;
	
	// cleanup the all states
	while (!states.empty())
	{
		states.back()->Cleanup();

		delete states.back();
		states.pop_back();
	}

	std::cout << "AppEngine cleaned up." << std::endl;
}

void AppEngine::ChangeState(AppState* state)
{
	if (!states.empty()) 
	{
		states.back()->Cleanup();

		delete states.back();
		states.pop_back();
	}

	std::cout << "changing state" << std::endl;

	states.push_back(state);
	states.back()->Init(this);
}

void AppEngine::PushState(AppState* state)
{
	// pause current state
	if (!states.empty())
		states.back()->Pause();

	std::cout << "pushing state" << std::endl;

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void AppEngine::PopState()
{
	// cleanup the current state
	if (!states.empty())
	{
		states.back()->Cleanup();

		delete states.back();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty())
		states.back()->Resume();
}

void AppEngine::PopState(int amount)
{
	for (size_t i = 0; i < amount; i++)
	{
		// cleanup the current state
		if (!states.empty())
		{
			states.back()->Cleanup();

			delete states.back();
			states.pop_back();
		}

		// resume previous state
		if (!states.empty())
			states.back()->Resume();
	}
}

void AppEngine::HandleEvents()
{
	if (running)
		states.back()->HandleEvents();
}

void AppEngine::Update()
{
	// let the state update the game
	if (running)
		states.back()->Update();
}

void AppEngine::Draw()
{
	// let the state draw the screen
	if (running)
		states.back()->Draw();
}

void AppEngine::Quit()
{
	std::cout << "AppEngine shutdown initiated." << std::endl;

	for (size_t i = 0; i < states.size(); i++)
	{
		states.back()->Cleanup();
		delete states.back();
		states.pop_back();
	}

	states.clear();

	running = false;
}
