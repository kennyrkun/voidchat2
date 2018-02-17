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
			inputBox.setOrigin(sf::Vector2f(inputBox.getLocalBounds().width / 2.0f, inputBox.getLocalBounds().height / 2.0f));

			{ sf::Vector2f pos;
			pos.x = static_cast<int>(window.getSize().x / 2.0f);
			pos.y = static_cast<int>(window.getSize().y - (inputBox.getLocalBounds().height / 2.0f));
			inputBox.setPosition(pos); }

			inputBox.setFillColor(sf::Color(100, 100, 100));
		}

		{
			std::cout << "send button" << std::endl;
			sendButton.setRadius(10);
			sendButton.setOrigin(sf::Vector2f(sendButton.getLocalBounds().width / 2, sendButton.getLocalBounds().height / 2));

			{ sf::Vector2f pos;
			pos.x = static_cast<int>((inputBox.getPosition().x * 2) - 20.0f);
			pos.y = static_cast<int>(inputBox.getPosition().y);
			sendButton.setPosition(pos); }

			if (sendButtonTexture.loadFromFile("resource\\textures\\sendbutton.png"))
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
			inputBoxText.setString("type a message");
			inputBoxText.setCharacterSize(14);
			inputBoxText.setOrigin(0, inputBoxText.getLocalBounds().height / 2);

			{ sf::Vector2f pos;
			pos.x = 5;
			pos.y = static_cast<int>(inputBox.getPosition().y); 
			inputBoxText.setPosition(pos); }

			inputBoxText.setFillColor(sf::Color::White);
		}

		{
			std::cout << "typing text" << std::endl;
			typingIdicator.setFont(font);
			typingIdicator.setString("name" " is typing");
			typingIdicator.setCharacterSize(12);
			typingIdicator.setOrigin(sf::Vector2f(typingIdicator.getLocalBounds().width / 2.0f, typingIdicator.getLocalBounds().height / 2.0f));

			{ sf::Vector2f pos;
			pos.x = static_cast<int>((typingIdicator.getLocalBounds().width / 2.0f) + 4.0f);
			pos.y = static_cast<int>(inputBox.getPosition().y - 24.0f);
			typingIdicator.setPosition(pos); }

			typingIdicator.setFillColor(sf::Color::White);
		}

		//std::cout << "done!\n" << std::endl;
	} // text

	std::cout << "finished." << std::endl;
}

void VoidChat::Render()
{
	window.clear(sf::Color(58, 58, 58));

	window.setView(scrollableView);

//	for (size_t i = messages.size(); i > 0; i--)
	for (size_t i = 0; i < messages.size(); i++)
	{
		window.draw(messages[i]);
	}

	// ui elements
	window.setView(window.getDefaultView());
	if (isClientTyping)
		window.draw(typingIdicator);
	window.draw(inputBox);
	window.draw(sendButton);
	window.draw(inputBoxText);
	
	window.display();
}

void VoidChat::Update(sf::Event &e)
{
	setIsTyping(true);

	if (e.text.unicode < 128) // something on a keyboard
	{
		std::string message = inputBoxText.getString(); // temp string

		if (e.text.unicode == 13) // return key
		{
			if (message.length() != 0) // can't send nothing, can we?
			{
				std::cout << "sent \"" << message << "\" (length " << message.length() << ")" << std::endl; // send message comes here

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
				{
					newMessage.setPosition(8, inputBox.getPosition().y - 60);
				}

				if (!sendMessageToServer(message))
				{
					std::cout << "failed to send message" << std::endl;
					newMessage.setFillColor(sf::Color(255, 255, 255, 100));
				}
				else
				{
					newMessage.setFillColor(sf::Color(200, 200, 200, 255));
				}

				messages.push_back(newMessage);
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
}

void VoidChat::Main()
{
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::EventType::MouseWheelMoved)
			{
				//TODO: implement scrollbar and scrollstep

				if (event.mouseWheel.delta > 0) // down
				{
					scrollableView.move(0, -26);
				}
				else if (event.mouseWheel.delta < 0) // up
				{
					scrollableView.move(0, 20);
				}
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
