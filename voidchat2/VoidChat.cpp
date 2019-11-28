#include "VoidChat.hpp"

#include <string>

// public:

VoidChat::VoidChat()
{
	// nothing
}

VoidChat::~VoidChat()
{
	// nothing
}

void VoidChat::setIsTyping(bool typing)
{
	if (typing)
	{
		sendButton.setFillColor(sf::Color(190, 190, 190));
		onStartTyping();
	}
	else
	{
//		sendButton.setFillColor(sf::Color(125, 125, 125));
		sendButton.setFillColor(sf::Color(190, 190, 190, 50));
		onStopTyping();
	}
}

void VoidChat::Init()
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
			std::cout << "input box" << std::endl;
			inputBox.setSize(sf::Vector2f(250.0f, 36.0f)); // this is at 36 so that it devides evenly, because if it doesn't all the fonts and images look terrible.
			inputBox.setFillColor(sf::Color(100, 100, 100));
			inputBox.setPosition(sf::Vector2f(0, window.getSize().y - inputBox.getSize().y));
		}

		{
			std::cout << "send button" << std::endl;
			sendButton.setRadius(10);
			sendButton.setOrigin(sf::Vector2f(sendButton.getLocalBounds().width / 2, sendButton.getLocalBounds().height / 2));

			{ sf::Vector2f pos;
			pos.x = static_cast<int>((inputBox.getPosition().x * 2) - 20.0f);
			pos.y = static_cast<int>(inputBox.getPosition().y);
			sendButton.setPosition(pos); }

			if (sendButtonTexture.loadFromFile("resource/textures/sendbutton.png"))
				sendButton.setTexture(&sendButtonTexture);
			else
				sendButton.setFillColor(sf::Color(190, 190, 190, 50));
		}

		std::cout << "done!\n" << std::endl;
	} // ui

	{ // text
		std::cout << "setting texts" << std::endl;

		{ // font
			std::cout << "loading font" << std::endl;

			if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
				abort();
		} // font

		{
			std::cout << "input box text" << std::endl;
			inputBoxText.setFont(font);
			inputBoxText.setCharacterSize(14);
			inputBoxText.setString("type a message");
			inputBoxText.setFillColor(sf::Color::White);

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

	socket = new sf::TcpSocket;

	if (socket->connect(sf::IpAddress::LocalHost, 123) != sf::Socket::Status::Done)
	{
		std::cerr << "fuck" << std::endl;
		abort();
	}

	selector.add(*socket);

	running = true;
}

void VoidChat::HandleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			window.close();
		}
		//TODO: resize events
		else if (event.type == sf::Event::EventType::MouseWheelMoved)
		{
			//TODO: implement scrollbar and scrollstep

			if (event.mouseWheel.delta > 0) // down
				scrollableView.move(0, -26);
			else if (event.mouseWheel.delta < 0) // up
				scrollableView.move(0, 26);
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			onKeyPressed(event);
		}
		else if (event.type == sf::Event::TextEntered)
		{
			onTextEntered(event);
		}
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

// private:

int VoidChat::onSendMessage(std::string message)
{
	sf::Packet packet;
	packet << "outgoingMessage";
	packet << clientUsername;
	packet << message;

	return onSend(packet);
}

void VoidChat::onReceiveMessage(std::string author, std::string message)
{
	addMessage(author, message);
}

int VoidChat::onStartTyping()
{
	sf::Packet packet;
	packet << "startedTyping";
	packet << clientUsername;

	return onSend(packet);
}

int VoidChat::onStopTyping()
{
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

	return true;
}

int VoidChat::onKeyPressed(sf::Event& event)
{
	std::string message = inputBoxText.getString().toAnsiString();

	if (event.key.code == sf::Keyboard::Key::Return)
	{
		if (message.length() != 0) // can't send nothing, can we?
		{
			std::cout << "sent \"" << message << "\" (length " << message.length() << ")" << std::endl; // send message comes here

			addMessage(clientUsername, message);

			if (!onSendMessage(message))
			{
				std::cout << "failed to send message" << std::endl;
//				newMessage.setFillColor(sf::Color::Red);
			}

			message.clear();
			inputBoxText.setString(message);
			setIsTyping(false);
		}
		else // length != 0
			std::cout << "cannot send an empty message" << std::endl;
	}
	else if (event.key.code == sf::Keyboard::Key::Backspace) // backspace
	{
		bool wasAlreadyEmpty = message.empty();

		if (message.length() != 0) // can't remove nothing
			message.pop_back();

		// if the message is empty now, we have stopped typing
		if (message.size() == 0 && !wasAlreadyEmpty)
			setIsTyping(false);

		inputBoxText.setString(message);
	}

	return 0;
}

int VoidChat::onTextEntered(sf::Event& e)
{
	if (e.text.unicode < 128) // something on a keyboard
	{
		std::string message = inputBoxText.getString(); // temp string

		if (e.text.unicode == 13) // return key (send key)
			return -1;
		else if (e.text.unicode == 8) // backspace
			return -1;
		else // regular characters
		{
			// if the message is empty when we start typing, we have started typing
			if (message.size() == 0)
				setIsTyping(true);

			message += static_cast<char>(e.text.unicode);
			inputBoxText.setString(message); // we should see our changes, yes?
		}
	}

	return 0;
}

int VoidChat::onQuit(sf::Event& event)
{
	return 0;
}

int VoidChat::onSend(sf::Packet packet)
{
	return (socket->send(packet) == sf::Socket::Status::Done);
}

std::string VoidChat::getTypingString()
{
	std::string typingString;

	if (remoteTypingUsers.size() == 1)
		typingString = remoteTypingUsers.front() + " is typing";
	else if (remoteTypingUsers.size() == 2)
		typingString = remoteTypingUsers.front() + " and " + remoteTypingUsers.back() + " are typing";
	else if (remoteTypingUsers.size() >= 10)
		typingString = "several people are typing";
	else if (remoteTypingUsers.size() >= 3)
	{
		std::cout << "three or more" << std::endl;

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
	else
		typingString = "";

	return typingString;
}

void VoidChat::addMessage(std::string author, std::string message)
{
	sf::Text newMessage;
	newMessage.setString(message);
	newMessage.setFont(font);
	newMessage.setCharacterSize(18);

	if (!messages.empty())
	{
		newMessage.setPosition(8, messages.back().getPosition().y + 26);
		scrollableView.move(0, 26);
	}
	else
		newMessage.setPosition(8, inputBox.getPosition().y - 36);

	messages.push_back(newMessage);
}
