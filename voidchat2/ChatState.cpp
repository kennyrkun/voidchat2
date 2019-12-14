#include "AppEngine.hpp"
#include "ChatState.hpp"

#include <SFML/Graphics.hpp>
#include <SFUI/Theme.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

ChatState::ChatState(sf::IpAddress ip, char port, std::string username, sf::RenderWindow* window) : chat(ip, port, username, window)
{
}

void ChatState::Init(AppEngine* app_)
{
	app = app_;
}

void ChatState::Cleanup()
{
}

void ChatState::Pause()
{
	std::cout << "ChatState Pause" << std::endl;
}

void ChatState::Resume()
{
	std::cout << "ChatState Resume" << std::endl;
}

void ChatState::HandleEvents()
{
	chat.HandleEvents();
}

void ChatState::Update()
{
	chat.Update();
}

void ChatState::Draw()
{
	chat.Draw();
}
