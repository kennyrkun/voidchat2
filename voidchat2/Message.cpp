#include "Message.hpp"

#include <SFML/Network.hpp>

sf::Packet& operator <<(sf::Packet& packet, const Message& message)
{
	packet << "outgoingMessage";
	packet << message.author;
	packet << message.content;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Message& message)
{
	// assume that the command object has already been extracted
	packet >> message.author;
	packet >> message.content;
	return packet;
}

Message::Message()
{
	// nothing
}

Message::Message(const std::string& author, const std::string& message)
{
	this->author = author;
	this->content = message;
}
