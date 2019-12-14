#ifndef CHAT_STATE_HPP
#define CHAT_STATE_HPP

#include "AppState.hpp"
#include "VoidChat.hpp"

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <thread>
#include <string>

class ChatState : public AppState
{
public:
	ChatState(sf::IpAddress ip, char port, std::string username, sf::RenderWindow* window);

	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	VoidChat chat;
};

#endif // !CHAT_STATE_HPP
