#include "AppEngine.hpp"
#include "InitialiseState.hpp"
#include "ServerJoinState.hpp"
#include "ChatState.hpp"

#include <SFML/Graphics.hpp>
#include <SFUI/Theme.hpp>

#include <vector>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void InitialiseState::Init(AppEngine* app_)
{
	initTime.restart();
	std::cout << "IntialiseState Init" << std::endl;

	app = app_;

	// HACK: this is nasty
//	font = GBL::theme.getFont("Arial.ttf");
//	if (font == nullptr)
	{
		std::cerr << "failed to load provided arial, falling back to windows Arial!" << std::endl;

		if (!font.loadFromFile("C:/Windows/Fonts/Arial.ttf"))
			std::cerr << "failed to load font Arial!" << std::endl;
	}

	app->window.create(sf::VideoMode(400, 150), "voidchat" + app->settings.version, sf::Style::None);
	app->window.setVerticalSyncEnabled(app->settings.vsync);
	app->window.setTitle("voidchat initalising");

	initialiseText.setFont(font);
	initialiseText.setCharacterSize(56);
	initialiseText.setString("voidchat");
	initialiseText.setOrigin(sf::Vector2f(initialiseText.getLocalBounds().width / 2.0f, initialiseText.getLocalBounds().height - 20.0f));
	initialiseText.setPosition(sf::Vector2f(static_cast<int>(app->window.getView().getCenter().x), static_cast<int>(app->window.getView().getCenter().y / 2.0f)));

	thingthatdoesntripoffvisualstudio.setSize(sf::Vector2f(app->window.getSize().x, 20.0f));
	thingthatdoesntripoffvisualstudio.setFillColor(sf::Color(100, 100, 100));
	thingthatdoesntripoffvisualstudio.setPosition(sf::Vector2f(0.0f, (app->window.getSize().y - 20.0f)) );

	// draw once and then forget about it
	Draw();

	initialise();

	app->ChangeState(new AdminState);
}

void InitialiseState::Cleanup()
{
	std::cout << "Cleaning up IntialiseState" << std::endl;

	app->window.create(sf::VideoMode(500, 500), "voidchat" + app->settings.version, sf::Style::Default);
	app->window.setVerticalSyncEnabled(app->settings.vsync);

	/*
	sf::Image icon;
	if (icon.loadFromFile(GBL::DIR::textures + "get_app_1x.png"))
		newWindow->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	else
		derr("failed to load application icon");
	*/

	std::cout << "Initalisation took " << initTime.getElapsedTime().asSeconds() << "s" << std::endl;
	std::cout << "Cleaned up IntialiseState" << std::endl;
}

void InitialiseState::Pause()
{
	std::cout << "IntialiseState Pause" << std::endl;
}

void InitialiseState::Resume()
{
	std::cout << "IntialiseState Resume" << std::endl;
}

void InitialiseState::HandleEvents()
{
	sf::Event event;

	// TODO: allow movement of window

	while (app->window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			app->Quit();
			return;
		}
	}
}

void InitialiseState::Update()
{
}

void InitialiseState::Draw()
{

}

void InitialiseState::initialise()
{
	SFUI::Theme::loadFont("C:/Windows/Fonts/Arial.ttf");
	SFUI::Theme::loadTexture("./resource/textures/interface_square.png");

	SFUI::Theme::textCharacterSize = 11;
	SFUI::Theme::click.textColor = sf::Color::White;
	SFUI::Theme::click.textColorHover = sf::Color::White;
	SFUI::Theme::click.textColorFocus = sf::Color::White;
	SFUI::Theme::input.textColor = sf::Color::White;
	SFUI::Theme::input.textColorHover = sf::Color::White;
	SFUI::Theme::input.textColorFocus = sf::Color::White;
	SFUI::Theme::label.textColor = sf::Color::White;
	SFUI::Theme::label.textColorHover = sf::Color::White;
	SFUI::Theme::label.textColorFocus = sf::Color::White;
	SFUI::Theme::windowBgColor = sf::Color(80, 80, 80);
}
