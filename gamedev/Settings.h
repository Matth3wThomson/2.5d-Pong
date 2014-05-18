#pragma once

#include "Singleton.h"
#include <fstream>

#include <string>

#define SETTINGS_PATH "Resources//"




/**
*	A Class to represent the globally accessible settings of the game.
*	Contains methods to load in settings and can be extended as necessary
*/
class Settings : public Singleton<Settings>{

public:
	friend class Singleton<Settings>;

	void SetDefault(){
		this->SCREEN_WIDTH = 1280;
		this->SCREEN_HEIGHT = 720;

		this->FULLSCREEN = true;
		this->VSYNC = false;

		this->MUTED = false;

	}

	bool LoadSettings(const std::string& filename){

		std::ifstream f((SETTINGS_PATH + filename).c_str());

		if (!f.is_open()){
			printf("Settings file not found!");
			return false;
		}

		while (!f.eof()){
			std::string temp;
			f >> temp;

			if (temp == "resolution"){
				f >> SCREEN_WIDTH;
				f >> SCREEN_HEIGHT;
			} else if (temp == "fullscreen"){
				f >> FULLSCREEN;
			} else if (temp == "vsync"){
				f >> VSYNC;
			}
		}

		return true;
	}

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;



	bool FULLSCREEN;
	bool VSYNC;

	bool MUTED;

protected:
	Settings(){
		SetDefault();
	}
	virtual ~Settings(){

	}
};