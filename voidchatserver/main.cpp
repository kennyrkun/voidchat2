#include <SFML/Network.hpp>

#include <iostream>
#include <vector>

std::vector<sf::TcpSocket*> sockets;

bool testClientConnection(sf::TcpSocket* socket)
{
	std::cout << "Testing connecting to " << socket->getRemoteAddress() << std::endl;

	sf::Packet connectionTest;

	if (socket->send(connectionTest) == sf::Socket::Status::Disconnected)
		return false;
	else if (socket->getRemoteAddress() == sf::IpAddress::None)
		return false;

	return true;
}

void broadcastPacketToEveryoneExcept(sf::Packet packet, sf::TcpSocket* userToIgnore)
{
	for (auto& otherSocket : sockets)
	{
		if (otherSocket == userToIgnore)
			continue;

		otherSocket->send(packet);
	}
}

void broadcastPacketToEveryone(sf::Packet packet)
{
	for (auto& otherSocket : sockets)
		otherSocket->send(packet);
}

int main()
{
	std::cout << "VoidChatServer" << std::endl;

	sf::TcpListener listener;

	if (listener.listen(123) != sf::Socket::Status::Done)
	{
		std::cerr << "failed to bind listener to port" << std::endl;
		return -1;
//		abort();
	}
	else
		std::cout << "Listener bound to port " << listener.getLocalPort() << std::endl;

	sf::SocketSelector selector;
	selector.add(listener);

	std::cout << "this server's local ip address is: " << sf::IpAddress::getLocalAddress() << std::endl;
	std::cout << "this server's public ip address is: " << sf::IpAddress::getPublicAddress() << std::endl;

	bool running = true;
	
	while (running)
	{
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				std::cout << "listener is ready to accept a new client" << std::endl;

				sf::TcpSocket* newSocket = new sf::TcpSocket;

				if (listener.accept(*newSocket) == sf::Socket::Status::Done)
				{
					selector.add(*newSocket);
					sockets.push_back(newSocket);
					std::cout << "accepted a client" << std::endl;
				}
			}
			else
			{
				for (auto& socket : sockets)
				{
					if (selector.isReady(*socket))
					{
						sf::Packet packet;
						socket->receive(packet);

						std::string command;
						packet >> command;

						if (command == "userJoining")
						{
							std::string user;
							packet >> user;

							std::cout << user << " joined the channel" << std::endl;

							sf::Packet outgoingMessage;
							outgoingMessage << "userJoined";
							outgoingMessage << user;
							broadcastPacketToEveryoneExcept(outgoingMessage, socket);
						}
						else if (command == "userLeaving")
						{
							std::string user;
							packet >> user;

							std::cout << user << " started typing" << std::endl;

							sf::Packet outgoingMessage;
							outgoingMessage << "userLeft";
							outgoingMessage << user;

							broadcastPacketToEveryoneExcept(outgoingMessage, socket);
						}
						else if (command == "startedTyping")
						{
							std::string user;
							packet >> user;

							std::cout << user << " started typing" << std::endl;

							sf::Packet outgoingMessage;
							outgoingMessage << "userStartedTyping";
							outgoingMessage << user;

							broadcastPacketToEveryoneExcept(outgoingMessage, socket);
						}
						else if (command == "stoppedTyping")
						{
							std::string user;
							packet >> user;

							std::cout << user << " stopped typing" << std::endl;

							sf::Packet outgoingMessage;
							outgoingMessage << "userStoppedTyping";
							outgoingMessage << user;

							broadcastPacketToEveryoneExcept(outgoingMessage, socket);
						}
						else if (command == "outgoingMessage")
						{
							std::string author;
							std::string message;

							packet >> author;
							packet >> message;

							std::cout << author << ": " << message << std::endl;

							sf::Packet outgoingMessage;
							outgoingMessage << "incomingMessage";
							outgoingMessage << author;
							outgoingMessage << message;

							broadcastPacketToEveryoneExcept(outgoingMessage, socket);
						}
						else
							std::cerr << "unknown command from client: " << command << std::endl;

						break;
					}
				}
			}
		}

		sf::sleep(sf::milliseconds(100));
	}
}
