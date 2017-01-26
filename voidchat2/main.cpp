// 22 characters long until we make a new line

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>

using ConVar = bool;

sf::RenderWindow window(sf::VideoMode(250, 300), "VoidChat 2.0.0", sf::Style::Close);
sf::Event event;

static bool istyping;
static std::string username("Guest");

void Sleep(int ms)
{
	std::cout << "sleeping " << ms << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

namespace chat
{
	namespace ui
	{
		sf::Font font;
		sf::Text inputbox_message;
		sf::Text message_pos1;
		sf::Text message_pos2;
		sf::Text typing;
		sf::RectangleShape background;
		sf::RectangleShape inputBox;
		sf::RectangleShape menubutton;
		sf::CircleShape sendbutton;
		sf::Texture sendbutton_texture;

		void init()
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
				sendbutton.setRadius(10);
				sendbutton.setPosition(225, 275);
				sendbutton.setFillColor(sf::Color(125, 125, 125));

				if (!sendbutton_texture.loadFromFile("resource\\textures\\sendbutton.png"))
				{
					std::cout << "failed to load sendbutton texture, using default shape instead." << std::endl;

					sendbutton.setFillColor(sf::Color(125, 125, 125));
				}
				else
				{
					sendbutton.setTexture(&sendbutton_texture);
				}

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
				inputbox_message.setFont(font);
				inputbox_message.setString("type a message");
				inputbox_message.setCharacterSize(14);
				inputbox_message.setFillColor(sf::Color::White);
				inputbox_message.setPosition(4, 276);

				std::cout << "typing text" << std::endl;
				typing.setFont(font);
				typing.setString("name" " is typing");
				typing.setCharacterSize(12);
				typing.setFillColor(sf::Color::White);
				typing.setPosition(4, 258);

				std::cout << "chat history" << std::endl;
				message_pos1.setFont(font);
				message_pos1.setCharacterSize(14);
				message_pos1.setFillColor(sf::Color::White);
				message_pos1.setPosition(4, 235);

				message_pos2.setFont(font);
				message_pos2.setCharacterSize(14);
				message_pos2.setPosition(4, 215);

				message_pos1.setString("pos1");
				message_pos2.setString("pos2");

				std::cout << "\ndone!" << std::endl;	
			} // text

			std::cout << console_divider << std::endl;

			std::cout << "finished." << std::endl;

		}

		void sort_messages(std::string message)
		{
			message_pos1.setString(inputbox_message.getString());

			if (message_pos2.getString() != message_pos1.getString())
			{
				message_pos2.setString(message_pos1.getString());
			}
		}

		void update()
		{
			/*** UI COLOUR PALETTE ***/
			// background = 58, 58, 58
			// input box = 100, 100, 100
			// sendbutton-default = 125, 125, 125
			// sendbutton-useable = 190, 190, 190
			/*************************/

			{	// update the send button
				std::string message = ui::inputbox_message.getString();
				if (message.length() == 0)
				{
					ui::sendbutton.setFillColor(sf::Color(125, 125, 125));
				}
				else
				{
					ui::sendbutton.setFillColor(sf::Color(190, 190, 190));
				}
			}
		}
	}

	int message_send(std::string message)
	{
		return false; // TODO: send the message? networking. server. when the ui is done. and working.
	}

	void show_people_typing(int people_typing)
	{
		if (istyping)
		{
			if (people_typing > 1)
				ui::typing.setString("people are typing");
			else
				ui::typing.setString(username + " is typing");

			window.draw(ui::typing);
		}
		else
			return;
	}

	void handleinput()
	{

	}

	void getUserInput()
	{
		istyping = true;

		if (event.text.unicode < 128) // something on a keyboard
		{
			std::string message = ui::inputbox_message.getString(); // temp string

			if (event.text.unicode == 13) // return key
			{
				if (message.length() != 0) // can't sned nothing, can we?
				{
					std::cout << message << std::endl;

					ui::message_pos1.setFillColor(sf::Color(150, 150, 150));
					ui::message_pos1.setString(message); // TODO: this will be done by the server, hopefully.

					ui::sort_messages(message);

					if (!message_send(message))
					{
						std::cout << "failed to send message" << std::endl;
						ui::message_pos1.setFillColor(sf::Color(sf::Color::Red));
					}
					else
					{
						std::cout << "sending message \"" << message << "\" of length " << message.length() << std::endl; // send message comes here
						ui::message_pos1.setFillColor(sf::Color(200, 200, 200));
					}
				}
				else // length != 0
				{
					std::cout << "cannot send an empty message" << std::endl;
					return;
				}

				message.clear();
				istyping = false;
			}
			else if (event.text.unicode == 8) // backspace
			{
				if (message.length() != 0) // can't remove nothing
					message.resize(message.length() - 1);

				if (message.length() == 0) // not typing when nothing exists!
					istyping = false;
			}
			else // regular characters
				message += static_cast<char>(event.text.unicode);

			ui::inputbox_message.setString(message); // we should see our changes, yes?
		}
	}
}

void program_loop()
{
    while (window.isOpen())
    {
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.key.code == sf::Keyboard::Escape)
			{
				exit(0);
			}

			if (event.type == sf::Event::TextEntered)
			{
				chat::getUserInput();
			}
		}

		chat::ui::update();

//-------------------------------

		window.clear();

		window.draw(chat::ui::background);
		chat::show_people_typing(1);
		window.draw(chat::ui::inputBox);
		window.draw(chat::ui::inputbox_message);
		window.draw(chat::ui::sendbutton);
		window.draw(chat::ui::message_pos1);
		window.draw(chat::ui::message_pos2);

        window.display();
    }
}

int main()
{
	chat::ui::init();

	std::cout << "\n" "--program output--" "\n" << std::endl;

	program_loop();

    return 0;
}
