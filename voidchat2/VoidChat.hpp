#ifndef VOIDCHAT_CLIENT_HPP
#define VOIDCHAT_CLIENT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <list>

#define CVERSION "2.3.0"

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

//	void sortMessages(std::string message);
	void setIsTyping(bool typing);

	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning() { return running; }

private:
	std::list<std::string> remoteTypingUsers;

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

	int onSendMessage(std::string message);
	void onReceiveMessage(std::string author, std::string message);
	int onStartTyping();
	int onStopTyping();
	int onNetworkIncoming();
	int onKeyPressed(sf::Event& event);
	int onTextEntered(sf::Event& event);
	int onQuit(sf::Event& event);
	int onSend(sf::Packet packet);

	std::string getTypingString();
	void addMessage(std::string author, std::string message);

	sf::TcpSocket* socket;
	sf::SocketSelector selector;
};

#endif /* VOIDCHAT_CLIENT_HPP */
