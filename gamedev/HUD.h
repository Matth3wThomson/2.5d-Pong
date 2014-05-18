#pragma once

#include <sfml/Window.hpp>
#include <sfml/Graphics.hpp>

#include "SRenderer.h"
#include "HUDElement.h"
#include <list>

/**
*	A HUD class which holds a reference to all hud elements you pass it.
*	Ownership is NOT passed, and deletion must be handled by the object
*	creator.
*/
class HUD
{
public:

	/**
	*	The constructor for the hud class. Must be passed the window which
	*	it is to act in.
	*/
	HUD(const int& width, const int& height, sf::RenderWindow& window){
		this->window = &window;
		ortho = new SRenderer(width, height, true);
	};

	~HUD(void){ 

	};

	/**
	*	Ensures that all elements contained within the hud have their
	*	sfml window position aligned with their openGL position.
	*/
	inline void SetSize(const int& x, const int& y){
		int xOld = ortho->GetWidth();
		int yOld = ortho->GetHeight();

		float proportionX = ((float) x) / ((float) xOld); 
		float proportionY = ((float) y) / ((float) yOld);

		//The window offset is necessary as glViewport handles the object scaling. The offset is
		//the amount the sfml window coordinates must be multiplied by in order to line them up
		//with the glViewport modified locations.
		for (std::list<HUDElement*>::const_iterator i = elements.begin(); i != elements.end(); ++i){
			(*i)->SetWindowOffset(Vector2(proportionX, proportionY));
		}
	}

	Vector2 GetSize() const { return Vector2((float) ortho->GetWidth(), (float) ortho->GetHeight()); }
	sf::RenderWindow* GetWindow() const { return window; }

	/**
	*	Adds an element to the hud to be rendered by it.
	*/
	void AddElement(HUDElement& ele){
		elements.push_back(&ele);
	}

	/**
	*	NOT RECOMMENDED. Returns an element by its location in the hud. 
	*/
	HUDElement* GetElement(const int& location){
		std::list<HUDElement*>::iterator i = elements.begin();
		std::advance(i, location);
		return *i;
	}

	/**
	*	Removes an element from the hud, but does not delete it.
	*/
	void RemoveElement(HUDElement* element){
		std::list<HUDElement*>::iterator i = elements.begin();
		while (i != elements.end()){
			if ((*i) == element){
				elements.erase(i++);
			} else {
				++i;
			}
		}
	}

	/**
	*	Removes all elements from the hud, but does not delete them
	*/
	void RemoveAllElements(){
		list<HUDElement*>::iterator i = elements.begin();

		while (i != elements.end()){
			elements.erase(i++);
		}
	}

	/**
	*	Updates all elements according to their virtual update method
	*/
	void Update(const float& time){
		for (list<HUDElement*>::const_iterator i = elements.begin(); i != elements.end(); ++i){
			(*i)->Update(time);
		}
	}

	/**
	*	Updates all elements according to their virtual render method
	*/
	void Render() const{

		for (list<HUDElement*>::const_iterator i = elements.begin(); i != elements.end(); ++i){
			(*i)->Render(*ortho, *window);
		}
	}

protected:

	//The list of all elements to be renderered
	list<HUDElement*> elements;

	//The orthographic renderer that elements will be drawn to
	SRenderer* ortho;

	//The window in which the hud exists
	sf::RenderWindow* window;
};

