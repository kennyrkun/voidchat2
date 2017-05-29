#include "VoidChat.hpp"

#include <string>

// public:

VoidChat::VoidChat()
{

}

VoidChat::~VoidChat()
{

}

void VoidChat::setIsTyping(bool typing)
{
	// sendbutton-default = 125, 125, 125
	// sendbutton-useable = 190, 190, 190

	if (typing)
	{
		sendButton.setFillColor(sf::Color(190, 190, 190));
	}
	else
	{
		sendButton.setFillColor(sf::Color(125, 125, 125));
	}
}

void VoidChat::Init()
{
	const std::string console_divider(10, '-');

	std::cout << "initializing UI" << std::endl; std::cout << std::endl;

	{
		std::cout << "background" << std::endl;
		background.setPosition(0, 0);
		background.setSize(sf::Vector2f(250, 300));
		background.setFillColor(sf::Color(58, 58, 58));

		std::cout << "input box" << std::endl;
		inputBox.setPosition(0, 270);
		inputBox.setSize(sf::Vector2f(250, 30));
		inputBox.setFillColor(sf::Color(100, 100, 100));

		std::cout << "send button" << std::endl;
		sendButton.setRadius(10);
		sendButton.setPosition(225, 275);
		sendButton.setFillColor(sf::Color(125, 125, 125));

		if (!sendButtonTexture.loadFromFile("resource\\textures\\sendbutton.png"))
		{
			sendButton.setFillColor(sf::Color(125, 125, 125));
		}
		else
		{
			sendButton.setTexture(&sendButtonTexture);
		}

		std::cout << "menu button" << std::endl;
		menuButton.setSize(sf::Vector2f(25, 25));
		menuButton.setPosition(0, 10);
		menuButton.setFillColor(sf::Color(100, 100, 100));

		std::cout << "speak button" << std::endl;
		speakButton.setScale(100, 100);
		speakButton.setPosition(0, 0);
		speakButton.setFillColor(sf::Color::White);

		std::cout << "\ndone!" << std::endl;
	} // ui

	std::cout << console_divider << std::endl;

	{ // text
		std::cout << "setting texts" << std::endl;

		{ // font
			std::cout << "loading font" << std::endl;

			if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
			{
				std::cout << "failed to load font" << std::endl; abort();
			}
		} // font

		std::cout << "input box text" << std::endl;
		inputBoxText.setFont(font);
		inputBoxText.setString("type a message");
		inputBoxText.setCharacterSize(14);
		inputBoxText.setFillColor(sf::Color::White);
		inputBoxText.setPosition(4, 276);

		/* TODO: REDO WHEN CHAT HISTORY WORKS.
		std::cout << "typing text" << std::endl;
		typing.setFont(font);
		typing.setString("name" " is typing");
		typing.setCharacterSize(12);
		typing.setFillColor(sf::Color::White);
		typing.setPosition(4, 258);
		*/

		std::cout << "chat history" << std::endl;
		messagePos1.setFont(font);
		messagePos1.setCharacterSize(14);
		messagePos1.setFillColor(sf::Color::White);
		messagePos1.setPosition(4, 235);

		messagePos2.setFont(font);
		messagePos2.setCharacterSize(14);
		messagePos2.setPosition(4, 215);

		std::cout << "\ndone!" << std::endl;
	} // text

	std::cout << console_divider << std::endl;

	std::cout << "finished." << std::endl;
}

void VoidChat::Render()
{
	programWindow.clear(sf::Color(58, 58, 58));

	// ui elements
	programWindow.draw(background);
	programWindow.draw(inputBox);
	programWindow.draw(sendButton);
	programWindow.draw(speakButton);
//	programWindow.draw(menubutton);

	// text
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
	programWindow.create(sf::VideoMode(250, 300), "VoidChat 2.0.0", sf::Style::Close);

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
