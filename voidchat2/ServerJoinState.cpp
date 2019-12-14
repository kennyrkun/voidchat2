#include "AppEngine.hpp"
#include "ServerJoinState.hpp"
#include "ChatState.hpp"

#include <SFUI/SFUI.hpp>

#include <iostream>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

enum Callback
{
	Connect,
};

void ServerJoinState::Init(AppEngine* app)
{
	std::cout << "ServerJoinState Init." << std::endl;

	this->app = app;

	if (fs::exists("./resource/server.cfg"))
	{
		std::cout << "server is saved, joining" << std::endl;

		std::ifstream getserver("./resource/server.cfg", std::ios::in | std::ios::binary);

		if (!getserver.is_open())
			abort();

		std::string ipaddr, addrport, username;
		std::getline(getserver, ipaddr);
		std::getline(getserver, addrport);
		std::getline(getserver, username);

		app->ChangeState(new ChatState(sf::IpAddress(ipaddr), std::stoi(addrport), username, &app->window));
		return;
	}

	menu = new SFUI::Menu(app->window);
	menu->setPosition(sf::Vector2f(8, 10));

	menu->addLabel("Join a Server");

	SFUI::FormLayout* form = menu->addFormLayout();

	addressBox = new SFUI::InputBox();
	form->addRow("Address: ", addressBox);

	portBox = new SFUI::InputBox();
	form->addRow("Port: ", portBox);

	usernameBox = new SFUI::InputBox();
	form->addRow("Username: ", usernameBox);

	menu->addHorizontalBoxLayout();

	SFUI::HorizontalBoxLayout* hbox = menu->addHorizontalBoxLayout();

	hbox->addButton("Connect", Callback::Connect);
	hbox->addLabel("Save Server");
	hbox->add(rememberInformationBox = new SFUI::CheckBox);

	std::cout << "ServerJoinState Ready." << std::endl;
}

void ServerJoinState::Cleanup()
{
	std::cout << "Cleaing up ServerJoinState." << std::endl;

	delete menu;

	std::cout << "ServerJoinState cleaned up." << std::endl;
}

void ServerJoinState::Pause()
{
	std::cout << "ServerJoinState paused." << std::endl;
}

void ServerJoinState::Resume()
{
	std::cout << "ServerJoinState resumed." << std::endl;
}

void ServerJoinState::HandleEvents()
{
	sf::Event event;

	while (app->window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
			app->Quit();
		else if (event.type == sf::Event::EventType::Resized)
		{
			sf::Vector2u newSize(event.size.width, event.size.height);

			sf::FloatRect visibleArea(0.0f, 0.0f, event.size.width, event.size.height);
			app->window.setView(sf::View(visibleArea));
		}
		else if (event.type == sf::Event::EventType::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Key::Escape)
				app->PopState();
		}

		int id = menu->onEvent(event);

		switch (id)
		{
		case Callback::Connect:
		{
			sf::IpAddress ip = addressBox->getText();
			char port = std::stoi(portBox->getText().toAnsiString());
			std::string username = usernameBox->getText();

			if (rememberInformationBox->isChecked())
			{
				std::ofstream save("./resource/server.cfg", std::ios::out | std::ios::trunc | std::ios::binary);

				save << ip.toString() << std::endl;
				save << port << std::endl;
				save << username << std::endl;
			}

			app->PushState(new ChatState(ip, port, username, &app->window));
			break;
		}
		default:
			break;
		}
	}
}

void ServerJoinState::Update()
{
}

void ServerJoinState::Draw()
{
	app->window.clear(SFUI::Theme::windowBgColor);

	app->window.draw(*menu);

	app->window.display();
}
