#include "Notification.hpp"

#include <iostream>

Notification::Notification(const std::string& filename)
{
	soundbuffer.loadFromFile(filename);
	sound.setBuffer(soundbuffer);
}

void Notification::play()
{
	sound.play();
}
