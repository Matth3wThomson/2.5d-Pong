#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "HUD.h"
#include "SRenderer.h"
#include "EventEmitter.h"

/**
*	An abstract class intended for extension by game designer.
*	Represents an individual screen in the game. (eg. menu or game screen)
*/
class Screen : public EventEmitter, public EventListener
{
public:

	Screen(HUD* hud, SRenderer* proj){
		renderer = proj;
		this->hud = hud;
	};

	virtual ~Screen(void){ }

	virtual void LoadResources() = 0;
	virtual void DeleteResources() = 0;

	virtual void Update(const float& time) = 0;
	virtual void RenderScreen() = 0;

protected:

	SRenderer* renderer;
	HUD* hud;

private:
	Screen();
};

