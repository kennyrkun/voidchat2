#include "VoidChat.hpp"

#include "VisualMessage.hpp"
#include "Message.hpp"

#include <string>
#include <fstream>
#include <cstdlib>
#include <filesystem>

namespace fs = std::experimental::filesystem;

VoidChat::VoidChat(sf::IpAddress ip, char port, std::string username, sf::RenderWindow* window) : clientUsername(username)
{
	if (clientUsername.empty())
		clientUsername = "Guest";

	this->window = window;

	scrollableView.setSize(window->getDefaultView().getSize());
	scrollableView.setCenter(window->getDefaultView().getCenter());
	mainView.setSize(window->getDefaultView().getSize());
	mainView.setCenter(window->getDefaultView().getCenter());

	std::cout << "initializing UI" << std::endl;
	std::cout << std::endl;

	{
		{
			std::cout << "sounds" << std::endl;
			notifications["chat_message_inbound"] = new Notification("./resource/audio/chat_message_inbound.wav");
			notifications["neutral_connection_connected_currentchannel"] = new Notification("./resource/audio/neutral_connection_connected_currentchannel.wav");
			notifications["neutral_connection_disconnected_currentchannel"] = new Notification("./resource/audio/neutral_connection_disconnected_currentchannel.wav");
			notifications["neutral_connection_connectionlost_currentchannel"] = new Notification("./resource/audio/neutral_connection_connectionlost_currentchannel.wav");
			notifications["you_connected"] = new Notification("./resource/audio/you_connected.wav");
			notifications["you_disconnected"] = new Notification("./resource/audio/you_disconnected.wav");
			notifications["you_lost_connection"] = new Notification("./resource/audio/you_lost_connection.wav");
			notifications["you_error"] = new Notification("./resource/audio/you_error.wav");
		}

		{
			std::cout << "input box" << std::endl;
			inputBox.setSize(sf::Vector2f(window->getSize().x, 36.0f)); // this is at 36 so that it devides evenly, because if it doesn't all the fonts and images look terrible.
			inputBox.setFillColor(sf::Color(100, 100, 100));
			inputBox.setPosition(sf::Vector2f(0, window->getSize().y - inputBox.getSize().y));
		}
		
		{
			std::cout << "send button" << std::endl;
			sendButton.setRadius(10);
			sendButton.setOrigin(sf::Vector2f(sendButton.getLocalBounds().width / 2, sendButton.getLocalBounds().height / 2));

			sf::Vector2f pos((inputBox.getPosition().x + inputBox.getSize().x) - 20.0f, inputBox.getPosition().y + 20.0f);
			sendButton.setPosition(pos);

			if (sendButtonTexture.loadFromFile("resource/textures/sendbutton.png"))
				sendButton.setTexture(&sendButtonTexture);
			
			sendButton.setFillColor(sendButtonCannotSendColor);
		}

		std::cout << "done!\n" << std::endl;
	} // ui

	{ // text
		std::cout << "setting texts" << std::endl;

		{ // font
			std::cout << "loading font" << std::endl;

			if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
				std::abort();
		} // font

		{
			std::cout << "input box text" << std::endl;
			inputBoxText.setFont(font);
			inputBoxText.setCharacterSize(14);
			inputBoxText.setString("type a message");
			inputBoxText.setFillColor(sendButtonCannotSendColor);

			inputBoxText.setOrigin(0, inputBoxText.getLocalBounds().height / 2);
			inputBoxText.setPosition(sf::Vector2f(5, (window->getSize().y - (inputBox.getSize().y / 2))));
		}

		{
			std::cout << "typing text" << std::endl;
			typingIdicator.setFont(font);
			typingIdicator.setCharacterSize(12);
			typingIdicator.setString("You can't see me!");
			typingIdicator.setFillColor(sf::Color::White);

			typingIdicator.setOrigin(sf::Vector2f(0, typingIdicator.getLocalBounds().height));
			typingIdicator.setPosition(sf::Vector2f(inputBox.getPosition().x + 3, inputBox.getPosition().y - 5));

			typingIdicator.setString("");
		}

		//std::cout << "done!\n" << std::endl;
	} // text

	std::cout << "finished." << std::endl;

	if (fs::exists("./resources/server_override.cfg"))
	{
		std::ifstream getserver("./resource/server.cfg", std::ios::in | std::ios::binary);

		if (!getserver.is_open())
			abort();

		std::string ipaddr, addrport;
		std::getline(getserver, ipaddr);
		std::getline(getserver, addrport);

		ip = ipaddr;
		port = std::stoi(addrport);
	}

	socket = new sf::TcpSocket;

	if (socket->connect(sf::IpAddress(ip), port) != sf::Socket::Status::Done)
	{
		notifications["you_error"]->play();

		std::cerr << "fuck" << std::endl;
		sf::sleep(sf::seconds(1));
		abort();
	}

	sf::Packet packet;
	packet << "userJoining";
	packet << clientUsername;

	onSend(packet);

	selector.add(*socket);

	notifications["you_connected"]->play();

	running = true;
}

VoidChat::~VoidChat()
{
	sf::Packet packet;
	packet << "userLeaving";
	packet << clientUsername;

	onSend(packet);

	sf::Event event;
	onQuit(event);

	notifications["you_disconnected"]->play();
}

void VoidChat::setIsTyping(bool typing)
{
	if (typing)
	{
		inputBoxText.setString("");
		inputBoxText.setFillColor(sendButtonSendColor);

		sendButton.setFillColor(sendButtonSendColor);
		onStartTyping();
	}
	else 
	{
		inputBoxText.setFillColor(sendButtonCannotSendColor);
		inputBoxText.setString("type a message");

		sendButton.setFillColor(sendButtonCannotSendColor);
		onStopTyping();
	}

	meTyping = typing;
}

void VoidChat::HandleEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			onQuit(event);
			window->close();

			running = false;
		}
		else if (event.type == sf::Event::EventType::Resized)
		{
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			mainView = sf::View(visibleArea);
			scrollableView = sf::View(visibleArea);

			inputBox.setSize(sf::Vector2f(window->getSize().x, 36.0f)); // this is at 36 so that it devides evenly, because if it doesn't all the fonts and images look terrible.
			inputBox.setPosition(sf::Vector2f(0, window->getSize().y - inputBox.getSize().y));
			sendButton.setPosition((inputBox.getPosition().x + inputBox.getSize().x) - 20.0f, inputBox.getPosition().y + 20.0f);
		}
		else if (event.type == sf::Event::EventType::MouseWheelMoved)
		{
			//TODO: implement scrollbar and scrollstep

			if (event.mouseWheel.delta > 0) // down
				scrollableView.move(0, -30);
			else if (event.mouseWheel.delta < 0) // up
				scrollableView.move(0, 30);
		}
		else if (event.type == sf::Event::TextEntered)
			onTextEntered(event);
	}
}

void VoidChat::Update()
{
	onNetworkIncoming();
}

void VoidChat::Draw()
{
	window->clear(sf::Color(60, 60, 60));

	window->setView(scrollableView);

	for (auto& message : messages)
		window->draw(message);

	// ui elements
	window->setView(mainView);

	window->draw(inputBox);
	window->draw(sendButton);
	window->draw(inputBoxText);
	window->draw(typingIdicator);

	window->display();
}

int VoidChat::onNetworkIncoming()
{
	if (selector.wait(sf::milliseconds(10)))
	{
		if (selector.isReady(*socket))
		{
			std::cout << "processing network information" << std::endl;

			sf::Packet packet;

			if (socket->receive(packet) == sf::Socket::Disconnected)
			{
				notifications["you_lost_connection"]->play();

				std::cout << "server freaking died" << std::endl;

				sf::sleep(sf::seconds(1));
				exit(0);
			}

			std::string command;
			packet >> command;

			if (command == "youGotDisconnected")
			{
				std::cout << "disconnected from server" << std::endl;

				notifications["you_disconnected"]->play();

				// TODO: disconnect message might do well here
				window->close();

				sf::sleep(sf::seconds(1));
			}
			else if (command == "userJoined")
			{
				std::string user;
				packet >> user;

				addMessage(Message("SYSTEM", user + " joined the channel."));
				messages.back().setAuthorColor(sf::Color::Yellow);
				messages.back().setContentColor(sf::Color::White);

				if (notificationsAllowed)
					notifications["neutral_connection_connected_currentchannel"]->play();
			}
			else if (command == "userLeft")
			{
				std::string user;
				std::string reason;
				packet >> user;
				packet >> reason;

				addMessage(Message("SYSTEM", user + " left the channel."));
				messages.back().setAuthorColor(sf::Color::Yellow);
				messages.back().setContentColor(sf::Color::White);

				if (notificationsAllowed)
				{
					if (reason == "timedOut")
						notifications["neutral_connection_connectionlost_currentchannel"]->play();
					else
						notifications["neutral_connection_disconnected_currentchannel"]->play();
				}
			}
			else if (command == "userStartedTyping")
			{
				std::string user;
				packet >> user;

				remoteTypingUsers.emplace_back(user);
				typingIdicator.setString(getTypingString());
			}
			else if (command == "userStoppedTyping")
			{
				std::string user;
				packet >> user;

				auto it = std::find(remoteTypingUsers.begin(), remoteTypingUsers.end(), user);
				if (it != remoteTypingUsers.end())
					remoteTypingUsers.erase(it);

				typingIdicator.setString(getTypingString());
			}
			else if (command == "incomingMessage")
			{
				Message message;
				packet >> message;

				onReceiveMessage(message);
			}
			else
			{
				std::cerr << "unknown command from server: " << command << std::endl;
			}
		}
	}

	return 1;
}

int VoidChat::onSendMessage(const Message& message)
{
	sf::Packet messagePacket;
	messagePacket << message;

	// TODO: only show this if send was successful
	std::cout << "sent \"" << message.author << "\" (length " << message.content.length() << ")" << std::endl;

	return onSend(messagePacket);
}

void VoidChat::onReceiveMessage(const Message& message)
{
	addMessage(message).setContentColor(sf::Color::White);

	if (notificationsAllowed)
		if (!window->hasFocus())
		{
			notifications["chat_message_inbound"]->play();
			window->requestFocus();
		}
}

int VoidChat::onStartTyping()
{
	std::cout << "informing the server that we are now typing" << std::endl;

	sf::Packet packet;
	packet << "startedTyping";
	packet << clientUsername;

	return onSend(packet);
}

int VoidChat::onStopTyping()
{
	std::cout << "informing the server that we stopped typing" << std::endl;

	sf::Packet packet;
	packet << "stoppedTyping";
	packet << clientUsername;

	return onSend(packet);
}

int VoidChat::onTextEntered(sf::Event& e)
{
	std::string message = "";

	if (meTyping)
		message = inputBoxText.getString().toAnsiString(); // temp string

	if (e.text.unicode < 128) // something on a keyboard
	{
		switch (e.text.unicode)
		{
		case 13: // return
		{
			std::cout << "return key was pressed" << std::endl;

			if (message.length() <= 0) // can't send nothing, can we?
				return -1;

			if (message[0] == '/')
			{
				if (message.substr(0, std::string("/notifications").length()) == "/notifications")
				{
					message.erase(0, 15);
					std::cout << message << std::endl;

					if (message == "off")
						notificationsAllowed = false;
					else if (message == "on")
						notificationsAllowed = true;
					else
					{
						Message newMessage("SYSTEM", "Invalid argument");
						addMessage(newMessage);

						break;
					}

					Message newMessage("SYSTEM", "Notifications are " + std::string(notificationsAllowed ? "enabled" : "disabled"));
					addMessage(newMessage);
				}
				else if (message.substr(0, std::string("/nick").length()) == "/nick" || message.substr(0, std::string("/nickname").length()) == "/nickname")
				{
					std::cout << "user is setting their nickname" << std::endl;

					// TODO: this could be a bug
					// what happens if there is no space?
					message.erase(0, message.find_first_of(' ') + 1);

					// TODO: make this a broadcast message
					Message newMessage("SYSTEM", clientUsername + " is now known as " + message);
					addMessage(newMessage);

					setIsTyping(false);

					clientUsername = message;
				}
				else
				{
					Message newMessage("SYSTEM", "Unkown command \"" + message.substr(1) + "\"");
					addMessage(newMessage);
				}

				inputBoxText.setString("");
				setIsTyping(false);
			}
			else
			{
				std::cout << "no command preceeding message" << std::endl;

				Message newMessage(clientUsername, message);
				addMessage(newMessage);

				if (!onSendMessage(newMessage))
				{
					std::cerr << "failed to send message" << std::endl;
					messages.back().setContentColor(sf::Color::Red);
				}
				else
					messages.back().setContentColor(sf::Color::White);

				setIsTyping(false);
			}

			break;
		}
		case 8: // backspace	
		{
			if (message.size() > 0)
			{
				message.pop_back();
				inputBoxText.setString(message);

				// if the message is empty now, we have stopped typing
				if (message.size() == 0)
				{
					std::cout << "it's empty now" << std::endl;
					setIsTyping(false);
				}
			}

			break;
		}
		default:
		{
			if (!meTyping)
				setIsTyping(true);

			message += static_cast<char>(e.text.unicode);
			inputBoxText.setString(message); // we should see our changes, yes?
			break;
		}
		}
	}

	return 0;
}

int VoidChat::onQuit(sf::Event& event)
{
	std::cout << "informing the server that we are leaving" << std::endl;

	sf::Packet packet;
	packet << "userLeaving";
	packet << clientUsername;

	return onSend(packet);
}

int VoidChat::onSend(sf::Packet packet)
{
	std::cout << "sending packet" << std::endl;
	return (socket->send(packet) == sf::Socket::Status::Done);
}

std::string VoidChat::getTypingString()
{
	std::cout << "setting typing string" << std::endl;

	std::string typingString;

	if (remoteTypingUsers.size() == 1)
		typingString = remoteTypingUsers.front() + " is typing";
	else if (remoteTypingUsers.size() == 2)
		typingString = remoteTypingUsers.front() + " and " + remoteTypingUsers.back() + " are typing";
	else if (remoteTypingUsers.size() == 3)
	{
		int loops = 0;
		for (auto& user : remoteTypingUsers)
		{
			if (user == remoteTypingUsers.front())
				typingString += user;
			else if (user == remoteTypingUsers.back())
				typingString += ", and " + user + " are typing";
			else
				typingString += ", " + user;

			loops++;
		}

		std::cout << "new: " << typingString << std::endl;
	}
	else if (remoteTypingUsers.size() > 3)
		typingString = "several people are typing";
	else
		typingString = "";

	return typingString;
}

VisualMessage& VoidChat::addMessage(const Message& message)
{
	std::cout << "adding new message" << std::endl;

	VisualMessage newMessage(message, &font);

	if (!messages.empty())
	{
		VisualMessage& lastMessage = messages.back();

		if (newMessage.message.author == lastMessage.message.author)
			newMessage.setAvatarHidden(true);

		// TODO: replace 30 magic number with lastMessage.getSize().y + 10
		newMessage.setPosition(sf::Vector2f(8, lastMessage.getPosition().y + 30));
		scrollableView.move(0, 30);
	}
	else
		newMessage.setPosition(sf::Vector2f(8, inputBox.getPosition().y - 36));

	messages.push_back(newMessage);

	std::cout << "added message" << std::endl;

	return newMessage;
}
