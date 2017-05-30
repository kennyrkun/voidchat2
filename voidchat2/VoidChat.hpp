#ifndef VOIDCHAT_CLIENT_HPP
#define VOIDCHAT_CLIENT_HPP

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <string>
#include <vector>

#define CVERSION "2.1.0"
#define SVERSION "2.0.0"

class VoidChat
{
public:
	VoidChat();
	~VoidChat();

	std::string clientUsername = "Guest";
	sf::RenderWindow programWindow;
	bool isClientTyping;

	void sortMessages(std::string message);
	void setIsTyping(bool typing);
	void Init();
	void Update(sf::Event &e);
	void Render();
	void Main();

private:
	std::vector<std::string> messages;
	std::vector<std::string> chatHistory;
	sf::Font font;
	sf::Text inputBoxText;
	sf::Text messagePos1;
	sf::Text messagePos2;
	sf::RectangleShape background;
	sf::RectangleShape inputBox;
	sf::RectangleShape menuButton;
	sf::CircleShape sendButton;
	sf::CircleShape speakButton;
	sf::Texture sendButtonTexture;

	bool sendMessageToServer(std::string message);
};

#endif /* VOIDCHAT_CLIENT_HPP */
