#include <iostream>

#include "ChatBox.h"
#include "Client.h"
#include "PacketType.h"

int main()
{
	std::string name;
	std::cout<<"What's your name: ";
	std::getline(std::cin, name);
	std::cout<<"\n";
	Client client(name);

	unsigned short port;
	std::cout<<"What port? ";
	std::cin>>port;
	std::cin.get();

	std::cout<<"Server IP? (local host), (local), (public)";
	std::string IP;
	std::getline(std::cin, IP);
	
	sf::Socket::Status status;
	if(IP=="local host")
		status=client.connect(sf::IpAddress::LocalHost, port);
	else if(IP=="local")
		status=client.connect(sf::IpAddress::getLocalAddress(), port);
	else if(IP=="public")
		status=client.connect(sf::IpAddress::getPublicAddress(), port);
	else
	{
		std::cout<<"Unknown command\n";
		return -1;
	}
	if(status!=sf::Socket::Done)
	{
		std::cout<<"Sorry we couldn't connect\n";
		return -1;
	}
	client.send(INITIAL_NAME_DATA, name);


	sf::RenderWindow window(sf::VideoMode(800, 600), ":D");
	sf::Font font;
	font.loadFromFile("ubuntu.ttf");
	ChatBox chatBox(sf::Vector2f(50, 100), 500, 5, 20, 15, font);
	chatBox.setFillColor(sf::Color::Cyan);
	chatBox.setOutlineColor(sf::Color::Black);
	chatBox.setCharColor(sf::Color::Black);
	chatBox.connectOnEnter([&](const std::string & s){client.send(GENERAL_MSG, s);});

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type==sf::Event::Closed)
				window.close();
			chatBox.handleEvent(event);
		}
		std::string toBePushed;
		
		sf::Socket::Status status=client.receive(toBePushed);
		if(status==sf::Socket::Done)
		{
			std::cout<<toBePushed<<"\n";
			chatBox.push(toBePushed);
		}
		else if(status==sf::Socket::Disconnected)
		{
			chatBox.push("You have been disconnected");
			chatBox.draw(window);
			window.display();
			sf::sleep(sf::seconds(2));
			return -1;
		}


		window.clear(sf::Color::White);
		chatBox.update();
		chatBox.draw(window);
		window.display();
	}
}
