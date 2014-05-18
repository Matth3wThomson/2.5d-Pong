#include "Game.h"


void Game::ReceiveEvent(const string& event){

	//Example of events being queued
	if (event == "Menu"){

		eventToHandle = event;

	} else if (event == "Pong"){

		eventToHandle = event;

	} else if (event == "Quit"){

		eventToHandle = event;

	}
};

void Game::HandleEvent(){

	//Example of events being handled accordingly.
	if (eventToHandle == "Pong"){

		currentScreen->DeleteResources();
		delete currentScreen;

		currentScreen = new Pong(renderer, hud, 10);
		currentScreen->RegisterForEvent("Menu", *this);
		currentScreen->RegisterForEvent("Pong", *this);
		currentScreen->RegisterForEvent("Quit", *this);
		currentScreen->LoadResources();

	} else if (eventToHandle == "Menu"){

		currentScreen->DeleteResources();
		delete currentScreen;

		currentScreen = new TitleMenu(renderer, hud);
		currentScreen->RegisterForEvent("Pong", *this);
		currentScreen->RegisterForEvent("Quit", *this);
		currentScreen->LoadResources();

	} else if (eventToHandle == "Quit"){

		currentScreen->DeleteResources();
		delete currentScreen;

		quit = true;

	}

	eventToHandle = "";
}

