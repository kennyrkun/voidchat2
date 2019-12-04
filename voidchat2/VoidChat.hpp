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

class Message;

class VoidChat
{
public:
	VoidChat();
	~VoidChat();

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
	bool meTyping = false;

	std::vector<sf::Text> messages;
	sf::Font		   font;
	sf::Text		   inputBoxText;
	sf::Text		   typingIdicator;
	sf::RectangleShape background;
	sf::RectangleShape inputBox;
	sf::CircleShape	   sendButton;
	sf::Texture        sendButtonTexture;
	sf::View           scrollableView;

	sf::SoundBuffer notificationSoundBuffer;
	sf::Sound notificationSound;

	bool running;

	int onSendMessage(const Message& message);
	void onReceiveMessage(const Message& message);
	int onStartTyping();
	int onStopTyping();
	int onNetworkIncoming();
	int onTextEntered(sf::Event& event);
	int onQuit(sf::Event& event);
	int onSend(sf::Packet packet);

	std::string getTypingString();
	void addMessage(const Message& message);

	sf::TcpSocket* socket;
	sf::SocketSelector selector;

	sf::Color sendButtonCannotSendColor = sf::Color(150, 150, 150);
	sf::Color sendButtonSendColor = sf::Color::White;
};

#endif // !VOIDCHAT_CLIENT_HPP
