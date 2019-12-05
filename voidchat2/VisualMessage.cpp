#include "VisualMessage.hpp"

VisualMessage::VisualMessage(const Message& message, sf::Font* font) : message(message)
{
	author.setFont(*font);
	content.setFont(*font);

	author.setString(message.author);
	author.setCharacterSize(12);
	author.setFillColor(sf::Color(180, 180, 180));
	content.setString(message.content);
	content.setCharacterSize(16);

	avatarTexture.loadFromFile("./resource/textures/avatars/" + message.author + ".png");

	avatar.setSize(sf::Vector2f(24, 24));
	avatar.setTexture(&avatarTexture, true);
}

VisualMessage::~VisualMessage()
{
}

void VisualMessage::setPosition(const sf::Vector2f& newPosition)
{
	boundingBox.top = newPosition.x;
	boundingBox.left = newPosition.y;

	avatar.setPosition(sf::Vector2f(boundingBox.top, boundingBox.left));
	author.setPosition(avatar.getPosition() + sf::Vector2f(avatar.getSize().x + 8, -4));
	content.setPosition(avatar.getPosition() + sf::Vector2f(avatar.getSize().x + 8 + (avatarHidden ? 24 : 0), avatarHidden ? 4 : 8));

	//boundingBox.width = avatar.getPosition().x;
	boundingBox.height = avatar.getSize().y;
}

sf::Vector2f VisualMessage::getPosition() const
{
	return sf::Vector2f(boundingBox.top, boundingBox.left);
}

sf::Vector2f VisualMessage::getSize() const
{
	return sf::Vector2f(boundingBox.width, boundingBox.height);
}

void VisualMessage::setAvatarHidden(bool hideAvatar)
{
	avatarHidden = hideAvatar;
	avatar.setSize(sf::Vector2f(0, 0));
	setPosition(getPosition());
}

void VisualMessage::setContentColor(const sf::Color& color)
{
	content.setFillColor(color);
}

void VisualMessage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!avatarHidden)
	{
		target.draw(avatar);
		target.draw(author);
	}

	target.draw(content);
}
