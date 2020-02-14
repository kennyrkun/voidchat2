#ifndef VOIDCHAT_CLIENT_HPP
#define VOIDCHAT_CLIENT_HPP

#include "Notification.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

#define CVERSION "2.5.0"

class VisualMessage;
class Message;

class VoidChat
{
public:
	VoidChat(sf::IpAddress ip, char port, std::string username, sf::RenderWindow* window);
	~VoidChat();

	// showWindow();
	// hideWinodw();

	std::string clientUsername = "Guest";
	bool notificationsAllowed = true;
	sf::RenderWindow* window;

//	void sortMessages(std::string message);
	void setIsTyping(bool typing);

	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning() { return running; }

private:
	std::list<std::string> remoteTypingUsers;
	bool meTyping = false;

	std::vector<VisualMessage> messages;
	sf::Font		   font;
	sf::Text		   inputBoxText;
	sf::Text		   typingIdicator;
	sf::RectangleShape background;
	sf::RectangleShape inputBox;
	sf::CircleShape	   sendButton;
	sf::Texture        sendButtonTexture;
	sf::View           scrollableView;
	sf::View           mainView;

	std::unordered_map<std::string, Notification*> notifications;

//	std::map<std::string, sf::Sound*> sounds;
//	std::map<std::string, sf::SoundBuffer*> soundbuffers;

	bool running;

	int onNetworkIncoming();

	int onSendMessage(const Message& message);
	void onReceiveMessage(const Message& message);
	int onStartTyping();
	int onStopTyping();
	int onTextEntered(sf::Event& event);
	int onQuit(sf::Event& event);
	int onSend(sf::Packet packet);

	std::string getTypingString();
	VisualMessage& addMessage(const Message& message);

	sf::TcpSocket* socket;
	sf::SocketSelector selector;

	sf::Color sendButtonCannotSendColor = sf::Color(150, 150, 150);
	sf::Color sendButtonSendColor = sf::Color::White;
};

#endif // !VOIDCHAT_CLIENT_HPP
