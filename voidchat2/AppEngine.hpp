#ifndef APP_ENGINE_HPP
#define APP_ENGINE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <vector>
#include <string>

class AppState;

struct AppSettings
{
	std::string version = "2.5-19w50a";
	std::string title = "voidchat " + version;
	bool vsync = true;
	bool debug = true;
	bool console = false;
	bool useAnimations = true;
	float animationScale = 1;
	int maxfps = 60;

	int timezoneOffset = -6;
	// TODO: this
	//bool use24HourTime = false;

	struct Server
	{
		sf::IpAddress serverIpAddress = sf::IpAddress::LocalHost;
		unsigned short serverPort = 12345;

		bool connected = false;
	} server;

	struct CommandLineArguments
	{
		int argc;
		char** argv;
	} launchOptions;
};

class AppEngine
{
public:
	void Init(AppSettings settings_);
	void Cleanup();

	// Note: code after these functions is still executed.
	void ChangeState(AppState* state);
	void PushState(AppState* state);
	void PopState();
	// amount: the amount of states to pop starting from the back
	void PopState(int amount);

	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning() { return running; }
	void Quit();

	AppSettings settings;

	sf::RenderWindow window;

	std::vector<AppState*> states;
	std::vector<sf::Event> events;

private:
	bool running;
};

#endif // !APP_ENGINE_HPP
