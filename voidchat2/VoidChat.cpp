#include "VoidChat.hpp"

#include "VisualMessage.hpp"
#include "Message.hpp"

#include <string>
#include <fstream>
#include <cstdlib>

VoidChat::VoidChat()
{
	std::cout << "username: ";
	std::getline(std::cin, clientUsername);

	window.create(sf::VideoMode(250, 350), ("VoidChat " CVERSION), sf::Style::Close);
	window.setVerticalSyncEnabled(true); // just using vsync istead of a fixed timestep because it's a chat program, it doesn't need a timestep.

	scrollableView.setSize(window.getDefaultView().getSize());
	scrollableView.setCenter(window.getDefaultView().getCenter());

	std::cout << "initializing UI" << std::endl;
	std::cout << std::endl;

	{
		{
			std::cout << "sounds" << std::endl;
			notificationSoundBuffer.loadFromFile("./resource/audio/you_were_poked.wav");
			notificationSound.setBuffer(notificationSoundBuffer);
		}

		{
			std::cout << "input box" << std::endl;
			inputBox.setSize(sf::Vector2f(250.0f, 36.0f)); // this is at 36 so that it devides evenly, because if it doesn't all the fonts and images look terrible.
			inputBox.setFillColor(sf::Color(100, 100, 100));
			inputBox.setPosition(sf::Vector2f(0, window.getSize().y - inputBox.getSize().y));
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
			inputBoxText.setPosition(sf::Vector2f(5, (window.getSize().y - (inputBox.getSize().y / 2))));
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

	std::ifstream getserver("./resource/server.cfg", std::ios::in | std::ios::binary);

	if (!getserver.is_open())
		abort();

	std::string ip, port;
	std::getline(getserver, ip);
	std::getline(getserver, port);

	char p = std::stoi(port);

	socket = new sf::TcpSocket;

	if (socket->connect(sf::IpAddress(ip), p) != sf::Socket::Status::Done)
	{
		std::cerr << "fuck" << std::endl;
		abort();
	}

	selector.add(*socket);

	running = true;
}

VoidChat::~VoidChat()
{
	socket->disconnect();
	delete socket;
}

void VoidChat::setIsTyping(bool typing)
{
	if (typing)
	{
		std::cout << "starting to type" << std::endl;

		inputBoxText.setString("");
		inputBoxText.setFillColor(sendButtonSendColor);

		sendButton.setFillColor(sendButtonSendColor);
		onStartTyping();
	}
	else 
	{
		std::cout << "message box is now empty" << std::endl;

		inputBoxText.setFillColor(sendButtonCannotSendColor);
		inputBoxText.setString("type a message");

		sendButton.setFillColor(sendButtonCannotSendColor);
		onStopTyping();
	}

	meTyping = typing;

	std::cout << "typing: " << typing << std::endl;
}

void VoidChat::HandleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
			window.close();
		//TODO: resize events
		else if (event.type == sf::Event::EventType::MouseWheelMoved)
		{
			//TODO: implement scrollbar and scrollstep

			if (event.mouseWheel.delta > 0) // down
				scrollableView.move(0, -26);
			else if (event.mouseWheel.delta < 0) // up
				scrollableView.move(0, 26);
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
	window.clear(sf::Color(60, 60, 60));

	window.setView(scrollableView);

	for (size_t i = 0; i < messages.size(); i++)
		window.draw(messages[i]);

	// ui elements
	window.setView(window.getDefaultView());

	window.draw(inputBox);
	window.draw(sendButton);
	window.draw(inputBoxText);
	window.draw(typingIdicator);

	window.display();
}

int VoidChat::onSendMessage(std::string message)
{
	sf::Packet messagePacket;
	messagePacket << Message(clientUsername, message);

	// TODO: only show this if send was successful
	std::cout << "sent \"" << message << "\" (length " << message.length() << ")" << std::endl;

	return onSend(messagePacket);
}

void VoidChat::onReceiveMessage(std::string author, std::string message)
{
	addMessage(author, message);
	notificationSound.play();
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

int VoidChat::onNetworkIncoming()
{
	if (selector.wait(sf::milliseconds(10)))
	{
		if (selector.isReady(*socket))
		{
			std::cout << "processing network information" << std::endl;

			sf::Packet packet;
			socket->receive(packet);

			std::string command;
			packet >> command;

			if (command == "userStartedTyping")
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
				/*
				Message message;
				packet >> message;

				onReceiveMessage(message.author, message.content);
				*/

				std::string author;
				std::string message;

				packet >> author;
				packet >> message;

				onReceiveMessage(author, message);
			}
			else
			{
				std::cerr << "unknown command from server: " << command << std::endl;
			}
		}
	}

	return 1;
}

int VoidChat::onTextEntered(sf::Event& e)
{
	std::cout << "processing text entered event" << std::endl;

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

			addMessage(clientUsername, message);

			if (!onSendMessage(message))
			{
				std::cerr << "failed to send message" << std::endl;
				messages.back().setFillColor(sf::Color::Red);
			}
			else
				messages.back().setFillColor(sf::Color::White);

			setIsTyping(false);
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

	sf::Packet leave;
	leave << "leaving";

	return onSend(leave);
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

void VoidChat::addMessage(std::string author, std::string message)
{
	std::cout << "adding new message" << std::endl;

	sf::Text newMessage;
	newMessage.setString(message);
	newMessage.setFont(font);
	newMessage.setCharacterSize(18);
	newMessage.setFillColor(sf::Color(100, 100, 100));

	if (!messages.empty())
	{
		newMessage.setPosition(8, messages.back().getPosition().y + 26);
		scrollableView.move(0, 26);
	}
	else
		newMessage.setPosition(8, inputBox.getPosition().y - 36);

	messages.push_back(newMessage);
}
