#ifndef VISUAL_MESSAGE_HPP
#define VISUAL_MESSAGE_HPP

#include "Message.hpp"

#include <SFML/Graphics.hpp>

class VisualMessage : public sf::Drawable
{
public:
	VisualMessage(const Message& message, sf::Font* font);
	~VisualMessage();

	void setPosition(const sf::Vector2f& newPosition);
	sf::Vector2f getPosition() const;

	sf::Vector2f getSize() const;

	void setAvatarHidden(bool hideAvatar);

	void setContentColor(const sf::Color& color);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	const Message message;

private:
	sf::Text author;
	sf::Text content;

	sf::RectangleShape avatar;
	sf::Texture avatarTexture;

	sf::FloatRect boundingBox;

	bool avatarHidden;
};

#endif // !VISUAL_MESSAGE_HPP
