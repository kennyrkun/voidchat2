#include "ChatBox.h"
#include <iostream>


ChatBox::ChatBox(const sf::Vector2f pos, const float length_, const float thickness_, 
				 const int charSize_, const std::size_t historyLength_, const sf::Font & font):
thickness(thickness_), charSize(charSize_), historyLength(historyLength_), length(length_),
box1(sf::Vector2f(length, thickness*(historyLength)+charSize*historyLength)),
box2(sf::Vector2f(length, thickness*2+charSize))
{
	box1.setPosition(pos);
	float x2=box1.getGlobalBounds().left;
	float y2=box1.getGlobalBounds().top + box1.getGlobalBounds().height+3*thickness;
	box2.setPosition(x2, y2);

	box1.setOutlineThickness(thickness);
	box2.setOutlineThickness(thickness);

	text.setFont(font);
	text.setCharacterSize(charSize);
	text.setPosition(x2+thickness, y2+thickness);

	historyText.setFont(font);
	historyText.setCharacterSize(charSize);

	
}

ChatBox::~ChatBox(void)
{
}

void ChatBox::update()
{
	text.setString(buffer);
}

void ChatBox::handleEvent(sf::Event & event)
{
	if(event.type==sf::Event::TextEntered)
	{
		char code=static_cast<char>(event.text.unicode);
		
		if(event.text.unicode==13)//enter
		{
			onEnter(buffer);
			buffer.clear();
		}

		else if(code!='\b')
			buffer.push_back(code);
		else if(code=='\b')
		{
			if(buffer.size()>0)
				buffer.pop_back();
		}
	}
}

void ChatBox::push(const std::string & s)
{
	if(s.size()>0)
	{
		history.push_front(s);
		if(history.size() > historyLength)
			history.pop_back();
	}
}

void ChatBox::connectOnEnter(std::function<void(const std::string & s)> func)
{
	onEnter=func;
}

void ChatBox::draw(sf::RenderWindow & window)
{
	window.draw(box1);
	window.draw(box2);
	window.draw(text);

	float x = box1.getGlobalBounds().left + 2 * thickness;
	float y = box1.getGlobalBounds().top + box1.getGlobalBounds().height-thickness-charSize;
	
	for(std::size_t i=0; i < history.size(); ++i)
	{
		historyText.setString(history[i]);
		historyText.setPosition(x, y - i * charSize - i * thickness - thickness);
		window.draw(historyText);
	}
}


void ChatBox::setOutlineColor(const sf::Color & color)
{
	box1.setOutlineColor(color);
	box2.setOutlineColor(color);
}

void ChatBox::setFillColor(const sf::Color & color)
{
	box1.setFillColor(color);
	box2.setFillColor(color);
}

void ChatBox::setCharColor(const sf::Color & color)
{
	text.setColor(color);
	historyText.setColor(color);
}
