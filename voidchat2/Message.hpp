#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <SFML/Network.hpp>

#include <string>

class Message
{
public:
	Message(const std::string& author, const std::string& message);

	std::string author;
	std::string messageContent;

	// TODO: time sent
};

sf::Packet& operator <<(sf::Packet& packet, const Message& command);
sf::Packet& operator >>(sf::Packet& packet, Message& command);

#endif // !MESSAGE_HPP
