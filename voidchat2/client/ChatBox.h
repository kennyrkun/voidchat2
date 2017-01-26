#pragma once

#include <deque>
#include <string>
#include "SFML/Graphics.hpp"
#include <functional>

namespace sf
{
	class Packet;
}

class ChatBox
{
private:
	const std::size_t historyLength;
	const float thickness;
	const int charSize;
	const float length;

	sf::RectangleShape box1;  //histories go here
	sf::RectangleShape box2;  //buffer goes here

	sf::Text text;
	sf::Text historyText;
	std::deque<std::string> history;

	std::string buffer;

	std::function<void(const std::string & s)> onEnter;
public:
	ChatBox(const sf::Vector2f pos,const float length, const float thickness, const int charSize, const std::size_t historyLength, const sf::Font & font);
	~ChatBox();

	void setOutlineColor(const sf::Color & color);
	void setFillColor(const sf::Color & color);
	void setCharColor(const sf::Color & color);



	void update();
	void handleEvent(sf::Event & event);
	
	void push(const std::string & s);
	void connectOnEnter(std::function<void(const std::string & s)> func);

	void draw(sf::RenderWindow & window);

};

