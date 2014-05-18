#pragma once

//Still quite messy, will be sorted towards finishing.
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>

#include "Settings.h"

#include "SRenderer.h"
#include "HUD.h"

#include "ResourcesManager.h"
#include "DebugHud.h"

#include "Game.h"

//Lets include our game icon!
extern "C"
{
#include "gameIcon.c"
}

//Properties to prevent unnecessary mouseMoved events!
int lastMousePosX, lastMousePosY;

int main(){

	//Load in settings from file
	Settings::Instance().LoadSettings("Settings.txt");

	//Create & configure the window with respect to the settings loaded
	sf::RenderWindow window;

	if (Settings::Instance().FULLSCREEN){
		window.create(sf::VideoMode(Settings::Instance().SCREEN_WIDTH, Settings::Instance().SCREEN_HEIGHT), "2.5d Pong", sf::Style::Fullscreen);
	} else {
		window.create(sf::VideoMode(Settings::Instance().SCREEN_WIDTH, Settings::Instance().SCREEN_HEIGHT), "2.5d Pong");
	}

	if (Settings::Instance().VSYNC){
		window.setVerticalSyncEnabled(true);
	}

	//Set our icon
	window.setIcon( pong.width, pong.height, pong.pixel_data );

	//Create the Renderer and HUD
	SRenderer r(Settings::Instance().SCREEN_WIDTH, Settings::Instance().SCREEN_HEIGHT, false);
	HUD hud(Settings::Instance().SCREEN_WIDTH, Settings::Instance().SCREEN_HEIGHT, window);

	//Create Resource Manager
	ResourcesManager::Create();
	InputHandler::Create();

	//Set the joystick threshold to prevent needless events occuring
	window.setJoystickThreshold(InputHandler::Instance().JOYSTICK_TOLERANCE);

	//Initialise glew + GL
	if (GLEW_OK != glewInit()){
		printf("ERROR INITIALIZING GLEW!");
		return -1;
	};

	//Load in the default shader by default
	ResourcesManager::Instance().AddShader("basic", "testVert.glsl", "testFrag.glsl");

	//Set the default background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//Enable Texture Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Create clock
	sf::Clock c;

	//Debugging properties that have little or no overhead being in
	//release
	bool polygonMode = false;
	bool devUI = false;

	//Create a profiler
	DebugHud dHud;

	//Start the game
	Game game(&r, &hud);

	//Start game loop
	while (!game.HasQuit()){

		//Refresh the input the inputhandler is aware of
		InputHandler::Instance().RefreshInput();

		//Handle Input, specific to window
		sf::Event event;

		//Here we update the input handler so that the whole program can be aware
		//of non-polled input events.
		while (window.pollEvent(event))
		{
			switch (event.type){
			case sf::Event::Closed:
				game.SetQuit(true);
				break;

			case sf::Event::Resized:
				glViewport(0,0,event.size.width, event.size.height);
				hud.SetSize(event.size.width, event.size.height);
				break;

			case sf::Event::JoystickConnected:
				InputHandler::Instance().ParseControllers();
				break;

			case sf::Event::JoystickDisconnected:
				InputHandler::Instance().ParseControllers();
				break;

			case sf::Event::KeyReleased:
				InputHandler::Instance().SetKeyReleased(event.key.code);
				break;

			case sf::Event::JoystickButtonReleased:
				InputHandler::Instance().MENU_MODE = true;
				InputHandler::Instance().SetButtonReleased(event.joystickButton.button);
				std::cout << event.joystickButton.button;
				break;

			case sf::Event::MouseButtonPressed:
				InputHandler::Instance().MENU_MODE = false;
				break;

			case sf::Event::MouseButtonReleased:
				InputHandler::Instance().MENU_MODE = false;
				InputHandler::Instance().SetMouseButtonReleased();
				break;

			case sf::Event::MouseMoved:
				if (lastMousePosX != event.mouseMove.x || lastMousePosY != event.mouseMove.y){
					lastMousePosX = event.mouseMove.x;
					lastMousePosY = event.mouseMove.y;

					InputHandler::Instance().MENU_MODE = false;
				}
				break;

			case sf::Event::JoystickMoved:
				InputHandler::Instance().SetAxisMoved(event.joystickMove.axis);
				InputHandler::Instance().MENU_MODE = true;
				break;

			case sf::Event::JoystickButtonPressed:
				InputHandler::Instance().MENU_MODE = true;
				InputHandler::Instance().SetButtonPressed(event.joystickButton.button);
				break;					

			case sf::Event::KeyPressed:
				InputHandler::Instance().MENU_MODE = true;
				InputHandler::Instance().SetKeyPressed(event.key.code);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3)){
					devUI = !devUI;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)){
					polygonMode = !polygonMode;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)){
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4)){
						game.SetQuit(true);
					}
				}
				break;

			}
		}

		//Polygon mode toggle
		if (polygonMode){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//Clear the back colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Obtain the time since the last frame
		float time = c.restart().asSeconds();

		//Update the game and render it
		game.Update(time);
		game.Render();

		//Draw the dev overlay if necessary
		if (devUI){
			dHud.Update(time);
			dHud.Draw(window);
		}

		//swap buffers!
		window.display();
	}

	//Release all resources
	ResourcesManager::Destroy();
	InputHandler::Destroy();

	return 0;

}
