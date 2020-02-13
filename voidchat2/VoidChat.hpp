#ifndef VOIDCHAT_CLIENT_HPP
#define VOIDCHAT_CLIENT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <string>
#include <list>
#include <map>

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

	sf::SoundBuffer notificationSoundBuffer;
	sf::SoundBuffer userJoinedSoundBuffer;
	sf::SoundBuffer userLeftSoundBuffer;
	sf::SoundBuffer userTimedoutSoundBuffer;
	sf::SoundBuffer connectedSoundBuffer;
	sf::SoundBuffer disconnectedSoundBuffer;
	sf::SoundBuffer connectionLostBuffer;
	sf::SoundBuffer errorSoundBuffer;
	sf::Sound userJoinedSound;
	sf::Sound userLeftSound;
	sf::Sound userTimedoutSound;
	sf::Sound notificationSound;
	sf::Sound connectedSound;
	sf::Sound disconnectedSound;
	sf::Sound connectionLostSound;
	sf::Sound errorSound;

//	std::map<std::string, sf::Sound*> sounds;
//	std::map<std::string, sf::SoundBuffer*> soundbuffers;

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
	VisualMessage& addMessage(const Message& message);

	sf::TcpSocket* socket;
	sf::SocketSelector selector;

	sf::Color sendButtonCannotSendColor = sf::Color(150, 150, 150);
	sf::Color sendButtonSendColor = sf::Color::White;
};

#endif // !VOIDCHAT_CLIENT_HPP
