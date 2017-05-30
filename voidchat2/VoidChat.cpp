#include "VoidChat.hpp"

#include <string>

#define D(MESSAGE) std::cout << #MESSAGE << std::endl;

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

		isClientTyping = true;
	}
	else
	{
//		sendButton.setFillColor(sf::Color(125, 125, 125));
		sendButton.setFillColor(sf::Color(190, 190, 190, 50));

		isClientTyping = false;
	}
}

void VoidChat::Init()
{
	programWindow.create(sf::VideoMode(250, 350), ("VoidChat " CVERSION), sf::Style::Close);
	programWindow.setVerticalSyncEnabled(true);

	std::cout << "initializing UI" << std::endl;
	std::cout << std::endl;

	{
		std::cout << "input box" << std::endl;
		inputBox.setSize(sf::Vector2f(250.0f, 36.0f));
		inputBox.setOrigin(sf::Vector2f(inputBox.getLocalBounds().width / 2.0f, inputBox.getLocalBounds().height / 2.0f));
		inputBox.setPosition(sf::Vector2f(programWindow.getSize().x / 2.0f, programWindow.getSize().y - inputBox.getLocalBounds().height / 2.0f));
		inputBox.setFillColor(sf::Color(100, 100, 100));

		std::cout << "send button" << std::endl;
		sendButton.setRadius(10);
		sendButton.setOrigin(sf::Vector2f(sendButton.getLocalBounds().width / 2, sendButton.getLocalBounds().height / 2));
		sendButton.setPosition(sf::Vector2f((inputBox.getPosition().x * 2) - 20.0f, inputBox.getPosition().y));

		if (sendButtonTexture.loadFromFile("resource\\textures\\sendbutton.png"))
		{
			sendButton.setTexture(&sendButtonTexture);
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

		std::cout << "input box text" << std::endl;
		inputBoxText.setFont(font);
		inputBoxText.setString("type a message");
		inputBoxText.setCharacterSize(14);
		inputBoxText.setOrigin(0, inputBoxText.getLocalBounds().height / 2);
		inputBoxText.setPosition(sf::Vector2f(5, inputBox.getPosition().y));
		inputBoxText.setFillColor(sf::Color::White);

		std::cout << "typing text" << std::endl;
		typingIdicator.setFont(font);
		typingIdicator.setString("name" " is typing");
		typingIdicator.setCharacterSize(12);
		typingIdicator.setOrigin(sf::Vector2f(typingIdicator.getLocalBounds().width / 2.0f, typingIdicator.getLocalBounds().height / 2.0f));
		
		std::cout << inputBox.getPosition().y;

		typingIdicator.setPosition(sf::Vector2f((typingIdicator.getLocalBounds().width / 2.0f) + 4.0f, inputBox.getPosition().y - 24.0f));
		typingIdicator.setFillColor(sf::Color::White);

		std::cout << "chat history" << std::endl;
		messagePos1.setFont(font);
		messagePos1.setCharacterSize(14);
		messagePos1.setFillColor(sf::Color::White);
		messagePos1.setPosition(sf::Vector2f(5, inputBox.getPosition().y - 50));

		//messagePos2.setFont(font);
		//messagePos2.setCharacterSize(14);
		//messagePos2.setPosition(5, 215);

		//std::cout << "done!\n" << std::endl;
	} // text

	std::cout << "finished." << std::endl;
}

void VoidChat::Render()
{
	programWindow.clear(sf::Color(58, 58, 58));

	// ui elements
	programWindow.draw(background);
	if (isClientTyping)
		programWindow.draw(typingIdicator);
	programWindow.draw(inputBox);
	programWindow.draw(sendButton);
	programWindow.draw(inputBoxText);

	// TODO: loop this
	programWindow.draw(messagePos1);
	
	programWindow.display();
}

void VoidChat::Update(sf::Event &e)
{
	setIsTyping(true);

	if (e.text.unicode < 128) // something on a keyboard
	{
		std::string message = inputBoxText.getString(); // temp string

		if (e.text.unicode == 13) // return key
		{
			if (message.length() != 0) // can't sned nothing, can we?
			{
//				std::cout << message << std::endl;

//				chat_history.front() = message;

				if (!sendMessageToServer(message))
				{
					std::cout << "failed to send message" << std::endl;
					messagePos1.setFillColor(sf::Color(sf::Color::Red));
				}
				else
				{
					std::cout << "sent \"" << message << "\" (length " << message.length() << ")" << std::endl; // send message comes here
					messagePos1.setFillColor(sf::Color(200, 200, 200));

					messages.push_back(message);
				}
			}
			else // length != 0
			{
				std::cout << "cannot send an empty message" << std::endl;
			}

			message.clear();
		}
		else if (e.text.unicode == 8) // backspace
		{
			if (message.length() != 0) // can't remove nothing
				message.pop_back();
		}
		else // regular characters
		{
			message += static_cast<char>(e.text.unicode);
		}

		inputBoxText.setString(message); // we should see our changes, yes?

		if (message.length() == 0)
			setIsTyping(false);
	}

	if (!messages.empty()) // what was this for?
	{
		messagePos1.setString(messages.back());
		messagePos2.setString(messages.back());
	}
}

void VoidChat::Main()
{
	while (programWindow.isOpen())
	{
		sf::Event event;
		while (programWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
				programWindow.close();
				exit(0);
			}
			else if (event.type == sf::Event::TextEntered)
			{
				Update(event);
			}
		}

		Render();
	}
}

// private:

bool VoidChat::sendMessageToServer(std::string message)
{
	return true; // server not implemented
}

/*
// ytho
#define P(MESSAGE) std::cout << #MESSAGE << std::endl;

namespace chat
{
		void sort_messages(std::string message)
	{
		messagePos1.setString(inputBoxText.getString());

		if (messagePos2.getString() != messagePos1.getString())
		{
			messagePos2.setString(messagePos1.getString());
		}
	}

	void istyping(bool typing)
	{
		// sendbutton-default = 125, 125, 125
		// sendbutton-useable = 190, 190, 190

		if (typing)
		{
			ui::sendButton.setFillColor(sf::Color(190, 190, 190));
		}
		else
		{
			ui::sendButton.setFillColor(sf::Color(125, 125, 125));
		}
	}

	void update()
	{
		istyping(true);

		if (event.text.unicode < 128) // something on a keyboard
		{
			std::string message = chat::ui::inputBoxText.getString(); // temp string

			if (event.text.unicode == 13) // return key
			{
				if (message.length() != 0) // can't sned nothing, can we?
				{
					std::cout << message << std::endl;

					//					chat_history.front() = message;

					if (!message_send(message))
					{
						std::cout << "failed to send message" << std::endl;
						chat::ui::messagePos1.setFillColor(sf::Color(sf::Color::Red));
					}
					else
					{
						std::cout << "sending message \"" << message << "\" of length " << message.length() << std::endl; // send message comes here
						chat::ui::messagePos1.setFillColor(sf::Color(200, 200, 200));

						messages.push_back(message);
					}
				}
				else // length != 0
				{
					std::cout << "cannot send an empty message" << std::endl;
				}

				message.clear();
			}
			else if (event.text.unicode == 8) // backspace
			{
				if (message.length() != 0) // can't remove nothing
					message.pop_back();
			}
			else // regular characters
			{
				message += static_cast<char>(event.text.unicode);
			}

			chat::ui::inputBoxText.setString(message); // we should see our changes, yes?

			if (message.length() == 0)
				istyping(false);
		}

		if (!messages.empty())
		{
			ui::messagePos1.setString(messages.back());
			ui::messagePos2.setString(messages.back());
		}
	}
}*/
