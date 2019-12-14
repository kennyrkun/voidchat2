#ifndef SERVER_JOIN_STATE_HPP
#define SERVER_JOIN_STATE_HPP

#include "AppState.hpp"

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/InputBox.hpp>
#include <SFUI/Checkbox.hpp>

class AdminState : public AppState
{
public:
	void Init(AppEngine* app);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	AppEngine* app;

	SFUI::Menu* menu;

	SFUI::InputBox* addressBox;
	SFUI::InputBox* portBox;
	SFUI::InputBox* usernameBox;
	SFUI::CheckBox* rememberInformationBox;
};

#endif // !SERVER_JOIN_STATE_HPP