#pragma once

#include "../nclgl/Vector2.h"
#include "SRenderer.h"
#include "MeshManager.h"

#include <sfml/Window/Mouse.hpp>
#include <sfml/Graphics/Font.hpp>
#include <sfml/Graphics/Text.hpp>
#include <sfml/Graphics/RenderWindow.hpp>

#define FONT_PATH "Resources\\Fonts\\"

class HUDElement
{
public:
	HUDElement(void){
		font = NULL;
		object = NULL;
		text = NULL;
		windowOffset = Vector2(1.0f,1.0f);
		z = 0;
	};

	/**
	*	The constructor for a hud element which is primarily an orthographic element. Less likely to have text.
	*/
	HUDElement(const Vector2& size, const Vector2& position, Shader* s, GLuint t, Mesh* m = NULL, std::string font = "");

	/**
	*	The constructor for a hud element that is primarily a text element. Less likely to have a custom mesh
	*/
	HUDElement(const Vector2& size, const Vector2& position, std::string font, Shader* s = NULL, GLuint t = 0, Mesh* m = NULL);

	/**
	*	The assignment operator for a HUDElement. DO NOT USE, DOES NOT WORK
	*/
	HUDElement operator=(const HUDElement& rhs);
	~HUDElement(void);

	/**
	*	The virtual update method for a hud element
	*/
	virtual void Update(const float& msec);

	/**
	*	The virtual render method for a hud element
	*/
	virtual void Render(SRenderer& ortho, sf::RenderWindow& window) const;

	/**
	*	For settings the orthographic z coordinate. DOES NOT APPLY TO TEXT!
	*/
	void SetZ(const float& z){
		this->z = z;
	}

	/**
	*	Set the 2D position of a HUD element
	*/
	void SetPosition(const Vector2& position){
		this->position = position;
		moved = true;
	}

	/**
	*	Set the size of the hud element
	*/
	void SetSize(const Vector2& size){
		this->size = size;
		moved = true;
	}

	/**
	*	Set the font size of the element. Will ensure there is
	*	a text element
	*/
	void SetFontSize(const int& size){
		if (text != NULL){
			text->setCharacterSize(size);
		}
	}

	/**
	*	Set the text of the element. Will ensure there
	*	is a text element
	*/
	void SetText(const string& text){
		if (this->text != NULL){
			overlay = text;
			this->text->setString(overlay);
			moved = true;
		}
	}

	/**
	*	Sets the colour of the text element. Will ensure
	*	there is a text element
	*/
	void SetTextColour(const sf::Color col){
		if (this->text != NULL){
			text->setColor(col);
		}
	}

	/**
	*	Sets the hud element to the bool supplied.
	*/
	void SetHighlighted(bool highlighted = true){
		if (highlighted){
			object->SetTexture(highlightedTexture);
		} else {
			object->SetTexture(texture);
		}
	}

	/**
	*	Sets the texture for the object to use when highlighted
	*/
	void SetHighlightTexture(GLuint texture){
		highlightedTexture = texture;
	}

	/**
	*	Sets the texture for the object
	*/
	void SetTexture(GLuint texture){
		object->SetTexture(texture);
	}

	/**
	*	Returns if the hud element is clicked. NOT RECOMMENDED as this uses
	*	direct polling of input device.
	*/
	bool IsClicked(sf::RenderWindow& window) const{
		if (IsMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			return true;
		}
		return false;
	}

	/**
	*	Returns whether the mouse is over this hud element.
	*/
	bool IsMouseOver(sf::RenderWindow& window) const{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);


		//Window offset is a safeguard for the user resizing the window. glViewport moves the elements accordingly, but
		//mouse location is a direct pixel. Prevents the elements "hitbox" moving unexpectedly after resizing.
		if ( mousePos.x > (position.x * windowOffset.x) && mousePos.x < ((position.x + size.x) * windowOffset.x) ){
			if ( mousePos.y > (position.y * windowOffset.y) && mousePos.y < ((position.y + size.y) * windowOffset.y) ){
				return true;
			}
		}

		return false;
	}

	inline float GetZ(){ return z; }
	inline Vector2 GetSize(){ return size; }
	inline Vector2 GetPosition(){ return position; }
	inline int GetFontSize(){ return text->getCharacterSize(); }
	inline Vector2 GetWindowOffset(){ return windowOffset; }
	inline string GetText(){ return text->getString(); }
	inline void SetWindowOffset(const Vector2& offset){
		windowOffset = offset;
	}

protected:

	//The size and position of the element
	Vector2 size;
	Vector2 position;

	//The orthographic z coordinate
	float z;

	//window offset for window resizing
	Vector2 windowOffset;

	//The string the text should contain
	std::string overlay;

	//Booleans to prevent needless updates
	bool moved;

	//The object textures
	GLuint texture;
	GLuint highlightedTexture;

	//Pointers to the render object and text and font elements
	RenderObject* object;
	sf::Font* font;
	sf::Text* text;

};

