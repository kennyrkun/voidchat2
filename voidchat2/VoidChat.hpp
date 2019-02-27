#ifndef VOIDCHAT_CLIENT_HPP
#define VOIDCHAT_CLIENT_HPP

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <string>
#include <vector>

#define CVERSION "2.2.0"

class VoidChat
{
public:
	VoidChat();
	~VoidChat();

	void Init();

	// showWindow();
	// hideWinodw();

	std::string clientUsername = "Guest";
	sf::RenderWindow window;
	bool isClientTyping;

//	void sortMessages(std::string message);
	void setIsTyping(bool typing);

	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning() { return running; }

private:
	std::vector<sf::Text> messages;
	sf::Font		   font;
	sf::Text		   inputBoxText;
	sf::Text		   typingIdicator;
	sf::RectangleShape background;
	sf::RectangleShape inputBox;
	sf::CircleShape	   sendButton;
	sf::Texture        sendButtonTexture;
	sf::View           scrollableView;

	bool running;

	int onSend(std::string message);
//	int onSend(sf::Event& event);
	int onRecieve(sf::Event& event);
	int onButtonPressed(sf::Event& event);
	int onQuit(sf::Event& event);
};

#endif /* VOIDCHAT_CLIENT_HPP */
