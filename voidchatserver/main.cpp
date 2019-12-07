#include <SFML/Network.hpp>

#include <iostream>
#include <list>

sf::TcpListener listener;
sf::SocketSelector selector;
std::list<sf::TcpSocket*> sockets;

void disconnectClient(sf::TcpSocket* socket, std::string reason = "Generic Disconnect")
{
	std::cout << "disconnecting " << socket->getRemoteAddress() << ": " << reason << std::endl;

	sf::Packet disconnect;
	disconnect << "youGotDisconnected";
	disconnect << reason;

	// don't bother checking for errors, this is just a courtesy
	socket->send(disconnect);

	selector.remove(*socket);
	socket->disconnect();
	sockets.remove(socket);
	delete socket;

	if (sockets.size() <= 0)
	{
		std::cout << "cleaning up while we have no clients" << std::endl;

		sockets.clear();
		selector.clear();
		selector.add(listener);
	}
}

bool detailedClientConnectionTest(sf::TcpSocket* socket)
{
	std::cout << "Testing connecting to " << socket->getRemoteAddress() << std::endl;

	sf::Packet connectionTest;

	if (socket->send(connectionTest) == sf::Socket::Status::Disconnected)
		return false;
	else if (socket->getRemoteAddress() == sf::IpAddress::None)
		return false;

	return true;
}

void broadcastMessage(sf::Packet packet, sf::TcpSocket* socketToIgnore = nullptr)
{
	for (auto& socket : sockets)
		if (socket != socketToIgnore)
			socket->send(packet);
}

int main()
{
	std::cout << "VoidChatServer" << std::endl;

	if (listener.listen(123) != sf::Socket::Status::Done)
	{
		std::cerr << "failed to bind listener to port" << std::endl;
		return -1;
//		abort();
	}
	else
		std::cout << "Listener bound to port " << listener.getLocalPort() << std::endl;

	selector.add(listener);

	std::cout << "this server's local ip address is: " << sf::IpAddress::getLocalAddress() << std::endl;
	std::cout << "this server's public ip address is: " << sf::IpAddress::getPublicAddress() << std::endl;

	bool running = true;
	
	while (running)
	{
		if (selector.wait(sf::seconds(10.0f)))
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

						if (socket->receive(packet) == sf::Socket::Disconnected)
						{
							std::cout << "client has disconnected" << std::endl;
							disconnectClient(socket, "Timed out");
							break;
						}

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
							broadcastMessage(outgoingMessage, socket);
						}
						else if (command == "userLeaving")
						{
							std::string user;
							packet >> user;

							std::cout << user << " left the channel" << std::endl;

							disconnectClient(socket, "You left.");

							sf::Packet outgoingMessage;
							outgoingMessage << "userLeft";
							outgoingMessage << user;
							outgoingMessage << "Manual Disconnect";

							broadcastMessage(outgoingMessage);
						}
						else if (command == "startedTyping")
						{
							std::string user;
							packet >> user;

							std::cout << user << " started typing" << std::endl;

							sf::Packet outgoingMessage;
							outgoingMessage << "userStartedTyping";
							outgoingMessage << user;

							broadcastMessage(outgoingMessage, socket);
						}
						else if (command == "stoppedTyping")
						{
							std::string user;
							packet >> user;

							std::cout << user << " stopped typing" << std::endl;

							sf::Packet outgoingMessage;
							outgoingMessage << "userStoppedTyping";
							outgoingMessage << user;

							broadcastMessage(outgoingMessage, socket);
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

							broadcastMessage(outgoingMessage, socket);
						}
						else
						{
							std::cerr << "unknown command from client: " << command << std::endl;

							if (!detailedClientConnectionTest(socket))
								disconnectClient(socket, "Connection Lost");
						}

						break;
					}
				}
			}
		}
	}
}
