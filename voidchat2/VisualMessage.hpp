#ifndef VISUAL_MESSAGE_HPP
#define VISUAL_MESSAGE_HPP

#include "Message.hpp"

#include <SFML/Graphics.hpp>

class VisualMessage : public sf::Drawable
{
public:
	VisualMessage();
	~VisualMessage();

	void setPosition();
	void getPosition();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Message message;
};

#endif // !VISUAL_MESSAGE_HPP
