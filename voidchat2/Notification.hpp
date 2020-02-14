#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <SFML/Audio.hpp>

#include <utility>

class Notification
{
public:
	Notification(const std::string& filename);

	void play();

private:
	sf::Sound sound;
	sf::SoundBuffer soundbuffer;
};

#endif // !NOTIFICATION_HPP
