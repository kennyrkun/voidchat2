#ifndef INITALISE_STATE_HPP
#define INITALISE_STATE_HPP

#include "AppState.hpp"

#include <SFML/Graphics.hpp>
#include <thread>

class InitialiseState : public AppState
{
public:
	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	AppEngine* app;
	// FIXME: shouldn't need to be a pointer

	bool isReady = false;

	void initialise();

	sf::Font font;
	sf::Text initialiseText;
	sf::RectangleShape thingthatdoesntripoffvisualstudio;

	sf::Clock initTime;
};

#endif // !INITALISE_STATE_HPP
