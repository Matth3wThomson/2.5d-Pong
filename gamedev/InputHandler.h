#pragma once

#include <SFML/Window.hpp>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "EventEmitter.h"
#include "Singleton.h"

/**
* A singleton used to represent the control schemes of each player in a game.
* Each time a controller is unplugged or plugged in, parse Controllers should
* be called.
*
* Each time any input device event is triggered in the main loop (using 
* window.pollEvent()) this class should be notified. Provides access to 
* key releasing and correctly timed presses to other areas of the program that
* require it. That is, they do not want to constantly poll the input device once per
* frame.
*
* This class also keeps track of input mode (MENU_MODE), unified joystick
* tolerance, and each players input scheme.
*/
class InputHandler : public EventEmitter, public Singleton<InputHandler>
{
	friend class Singleton<InputHandler>;

public:

	//SFML supports up to 8 controllers
	enum inputScheme {
		ARROWKEYS = -2, WASD = -1, CONTROLLER = 0,
		CONTROLLER2 = 1, CONTROLLER3 = 2, CONTROLLER4 = 3,
		CONTROLLER5 = 4, CONTROLLER6 = 5, CONTROLLER7 = 6, CONTROLLER8 = 7
	};

	/**
	*	Returns the number of joysticks that the input handler is aware of.
	*/
	inline int GetNoOfJoysticks(){
		return joysticks.size();
	}

	/**
	*	Returns the lowest SFML indexed joystick the inputhandler is aware of
	*   + i
	*/
	inline int GetJoystick(const int& i){
		return joysticks[i];
	}

	/**
	*	Parses SFML controllers. Makes the inputhandler aware of
	*	all SFML controllers
	*/
	int ParseControllers(){
		joysticks.clear();

		for (int i=0; i<8; ++i){
			if (sf::Joystick::isConnected(i)){
				joysticks.push_back(i);
			}
		}

		return joysticks.size();
	}

	/**
	*	Adds a player to the input handler with the scheme supplied
	*/
	void AddPlayer(const inputScheme& scheme){
		players.push_back(scheme);
	}

	/**
	*	Increments the players input scheme. That is iterates through
	*	all available input schemes until back to ARROW KEYS
	*/
	void IncrementPlayerScheme(const int& player){
		players[player] = (inputScheme) (players[player] + 1);

		if (players[player] >= GetNoOfJoysticks()){
			players[player] = ARROWKEYS;
		};
	}

	/**
	*	Sets a players input scheme to the one supplied.
	*/
	bool SetPlayer(const int& player, const inputScheme& scheme){
		if (scheme > ((signed int) joysticks.size())){
			return false;
		} else {
			players[player] = scheme;
			return true;
		}
	}

	/**
	*	Returns the number of players the input handler is currently
	*	aware is playing
	*/
	int GetNoOfPlayers(){
		return players.size();
	}

	/**
	*	Returns the input scheme of the player requested
	*/
	inputScheme GetPlayerScheme(const int& player){ 
		return players[player];
	}

	/**
	*	Returns a string equivalent of the players input scheme
	*/
	string GetPlayerSchemeString(const int& player){
		switch (players[player]){
		case -2:
			return "Arrow Keys";
			break;
		case -1:
			return "WASD";
			break;
		default:
			std::ostringstream str;
			str << "Controller: " << players[player];
			return str.str();
		}
	}

	/**
	*	Removes all the players from the inputHandler
	*/
	void ResetPlayers(){
		while (!players.empty()){
			players.pop_back();
		}
	}

	/**
	*	Resets the All buttons, keys, axes that the input handler was
	*	aware of being pressed or released
	*/
	void RefreshInput(){

		mouseButtonReleased = false;

		while (!keysPressed.empty()){
			keysPressed.pop_back();
		}

		while (!keysReleased.empty()){
			keysReleased.pop_back();
		}

		while (!buttonsPressed.empty()){
			buttonsPressed.pop_back();
		}

		while (!buttonsReleased.empty()){
			buttonsReleased.pop_back();
		}

		while (!axisMoved.empty()){
			axisMoved.pop_back();
		}
	}

	//Adds Key/Axis/JoystickButton to a list of them being pressed or released.
	void SetMouseButtonReleased(){ mouseButtonReleased = true; }
	void SetKeyPressed(sf::Keyboard::Key key){ keysPressed.push_back(key); };
	void SetKeyReleased(sf::Keyboard::Key key){ keysReleased.push_back(key); }
	void SetAxisMoved(sf::Joystick::Axis axis){ axisMoved.push_back(axis); }
	void SetButtonPressed(int button){ buttonsPressed.push_back(button); };
	void SetButtonReleased(int button){ buttonsReleased.push_back(button); }

	//Checks the correct list for whether the requested Key/Button/Axis has been pressed
	bool IsMouseButtonReleased(){ return mouseButtonReleased; }

	bool IsKeyPressed(sf::Keyboard::Key key){
		for (std::list<sf::Keyboard::Key>::const_iterator i = keysPressed.begin();
			i != keysPressed.end(); ++i){
				if ((*i) == key){
					return true;
				}
		}
		return false;
	}

	bool IsKeyReleased(sf::Keyboard::Key key){
		for (std::list<sf::Keyboard::Key>::const_iterator i = keysReleased.begin();
			i != keysReleased.end(); ++i){
				if ((*i) == key){
					return true;
				}
		}
		return false;
	}

	bool IsAxisMoved(sf::Joystick::Axis axis){
		for (std::list<sf::Joystick::Axis>::const_iterator i = axisMoved.begin();
			i!= axisMoved.end(); ++i){
				if ((*i) == axis){
					return true;
				}
		}
		return false;
	}

	bool IsButtonPressed(const int& button){
		for (std::list<int>::const_iterator i = buttonsReleased.begin();
			i != buttonsReleased.end(); ++i){
				if ((*i) == button){
					return true;
				}
		}
		return false;
	}

	bool IsButtonReleased(const int& button){ 
		for (std::list<int>::const_iterator i = buttonsReleased.begin();
			i != buttonsReleased.end(); ++i){
				if ((*i) == button){
					return true;
				}
		}
		return false;
	}

	float JOYSTICK_TOLERANCE;

	bool MENU_MODE;

protected:
	//A vector that contains all joysticks
	std::vector<int> joysticks;

	//A vector that contains all the players and their input schemes
	std::vector<inputScheme> players;

	//Constructor with default values
	InputHandler(void){
		ParseControllers();
		JOYSTICK_TOLERANCE = 20.0f;
		MENU_MODE = true;
	};

	~InputHandler(void){ };

	//For accessing when a mouse button has been released
	bool mouseButtonReleased;

	//Accessing key presses and released anywhere in the program
	std::list<sf::Keyboard::Key> keysPressed;
	std::list<sf::Keyboard::Key> keysReleased;

	//Accessing joystick button presses, button releases and axis
	//movements anywhere in the program. NOT JOYSTICK INDEPENDANT
	std::list<int>buttonsPressed;
	std::list<int> buttonsReleased;
	std::list<sf::Joystick::Axis> axisMoved;

};



