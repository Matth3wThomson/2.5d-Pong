#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "EventListener.h"
#include "InputHandler.h"

#define FONT_PATH "Resources\\Fonts\\"

/**
* This class represents the debugging overlay for the game.
* It currently handles FPS and controller axis reporting.
*/
class DebugHud : public EventListener
{
public:
	DebugHud();
	~DebugHud(void);

	void Update(float msec);
	void Draw(sf::RenderWindow& window);

	virtual void ReceiveEvent(const string& event){

	};
protected:
	//Fps stuff
	int nFrames;
	float time;
	int FPS;

	std::string overlay;

	sf::Font* font;
	sf::Text* text;
};

