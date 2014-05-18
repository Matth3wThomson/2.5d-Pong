#include "DebugHud.h"


DebugHud::DebugHud(void)
{
	font = NULL;
	text = NULL;

	font = new sf::Font();

	string typeface = "cour.ttf";

	if (!font->loadFromFile(FONT_PATH + typeface)){
		delete font;
		printf("Font loading failed!");


	} else {

		text = new sf::Text();

		text->setFont(*font);

		text->setCharacterSize(24);
		text->setColor(sf::Color(255, 255, 190, 255));
		text->setStyle(sf::Text::Bold);
	}

	time = 0;
	nFrames = 0;
	FPS = 0;

}


DebugHud::~DebugHud(void)
{
	if (text != NULL){
		delete text;
	}

	if (font != NULL){
		delete font;
	}
}

void DebugHud::Update(float msec){
	std::ostringstream str;

	time += msec;
	nFrames += 1;

	if (time > 1.0f){
		FPS = nFrames;
		nFrames = 0;
		time = 0;
	}

	str << "FPS: " << FPS << std::endl;

	for (int i=0; i<InputHandler::Instance().GetNoOfJoysticks(); ++i){
		str << "Controller " << i << std::endl;
		str << "X ax: " << 
			sf::Joystick::getAxisPosition(InputHandler::Instance().GetJoystick(i), sf::Joystick::X) << std:: endl;
		str << "Y ax: " <<
			sf::Joystick::getAxisPosition(InputHandler::Instance().GetJoystick(i), sf::Joystick::Y) << std::endl;
		str << "Z ax: " <<
			sf::Joystick::getAxisPosition(InputHandler::Instance().GetJoystick(i), sf::Joystick::Z) << std::endl;
	}

	overlay = str.str();
}

void DebugHud::Draw(sf::RenderWindow& window){
	text->setString(overlay);

	window.pushGLStates();
	window.draw(*text);
	window.popGLStates();

}