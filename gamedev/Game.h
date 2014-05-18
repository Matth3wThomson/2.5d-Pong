#pragma once

#include <string>
#include "TitleMenu.h"
#include "Pong.h"

/**
*	A class to represent the "game". This is responsible for
*	managing the game screens, and transferring between them
*	in response to received events.
*/
class Game : public EventEmitter, public EventListener
{
public:

	Game(SRenderer* renderer, HUD* hud){
		//Set our pointers to the renderer and hud
		this->renderer = renderer;
		this->hud = hud;
		quit = false;

		if (!InitializeGame()) std::cout << "Game Initialzation failed. Check HUD and Renderer!";

	}

	virtual ~Game(){}

	void SetRenderer(SRenderer* renderer){ this->renderer = renderer; }
	void SetHUD(HUD* hud){ this->hud = hud; }

	/**
	* The method responsible for initializing the
	* game. Here the first screen should be created
	*/
	bool InitializeGame(){
		if (!renderer && !hud){
			hud = NULL;
			renderer = NULL;
			return false;
		}

		//Set our current screen to the title menu
		currentScreen = new TitleMenu(this->renderer, this->hud);
		currentScreen->LoadResources();

		//Register for the event to start the game
		currentScreen->RegisterForEvent("Pong", *this);
		currentScreen->RegisterForEvent("Quit", *this);
		return true;
	}


	/**
	*	Overriden virtual method for responding to events.
	*	NOTE: Do not delete screens here, handle events in the
	*	handle event method.
	*/
	virtual void ReceiveEvent(const string& event);

	/**
	*	Method responsible for handling events that require
	*	deletion of objects which fired said events.
	*/
	void HandleEvent();

	/**
	*	Update the current screen
	*/
	void Update(const float& msec){
		currentScreen->Update(msec);
	}

	/**
	*	Render the current screen
	*/
	void Render(){
		currentScreen->RenderScreen();
	}

	/**
	*	Called every loop, handles any queued events
	*/
	bool HasQuit(){
		HandleEvent();
		return quit;
	}

	/**
	*	Call to end the game and close it
	*/
	void SetQuit(bool quit=true){
		this->quit = quit;
	}


protected:

	//Quit flag for the application
	bool quit;

	//The queued event to handle after the end of the iteration.
	string eventToHandle;

	//The renderer and hud classes to be used by screens. Can be modified!
	SRenderer* renderer;
	HUD* hud;

	//The current screen being displayed.
	Screen* currentScreen;
};

