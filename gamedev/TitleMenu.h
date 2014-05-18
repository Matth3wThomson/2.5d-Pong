#pragma once

#include "Screen.h"
#include "ResourcesManager.h"
#include "Pong.h"

/**
*	This class represents how the user should implement a title screen
*	with the engine
*/
class TitleMenu : public Screen
{
public:
	TitleMenu(SRenderer* proj, HUD* hud) : Screen(hud, proj){ };

	virtual ~TitleMenu(void);

	virtual void LoadResources();
	virtual void DeleteResources();
	virtual void Update(const float& time);
	virtual void RenderScreen();

	virtual void ReceiveEvent(const string& event){};

protected:

	//A const float for button padding
	static const float PADDING;

	//Values to keep track of menu driven interface
	int highlightedElement;
	int numberOfElements;

	//All of the elements present in our title menu
	HUDElement* startButton;
	HUDElement* quitButton;
	HUDElement* playerOneScheme;
	HUDElement* playerTwoScheme;
	HUDElement* muteButton;
	HUDElement* title;
	HUDElement* background;
	HUDElement* watermark;

};

