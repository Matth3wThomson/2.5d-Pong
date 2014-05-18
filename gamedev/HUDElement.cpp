#include "HUDElement.h"


HUDElement::HUDElement(const Vector2& size, const Vector2& position, Shader* s, GLuint t, Mesh* m, std::string font)
{
	//Set properties
	SetSize(size);
	SetPosition(position);

	windowOffset = Vector2(1.0f,1.0f);
	z = 0;

	//Custom mesh?
	if (m == NULL){
		m = MeshManager::Instance().AddMesh("TLquad");
	}

	//Create Render Object
	object = new RenderObject(m, s, t);

	//Create text and font object
	if (font != ""){
		this->font = new sf::Font();

		//If font loading worked, proceed
		if (this->font->loadFromFile(FONT_PATH + font)){

			text = new sf::Text();

			text->setFont(*this->font);
			text->setCharacterSize(24);
			text->setColor(sf::Color(255,255,255,255));
			SetText("");

		} else {
			delete this->font;
			this->font = NULL;
			this->text = NULL;
		}
	} else {
		this->font = NULL;
		this->text = NULL;
	}
}


HUDElement::HUDElement(const Vector2& size, const Vector2& position, std::string font, Shader* s, GLuint t, Mesh* m){
	SetSize(size);
	SetPosition(position);

	windowOffset = Vector2(1.0f,1.0f);
	z = 0;

	texture = t;
	highlightedTexture = t;

	this->font = new sf::Font();

	if (!this->font->loadFromFile(FONT_PATH + font)){
		this->font = NULL;
		delete this->font;
		printf("Font load failed HUD ELEMENT");
	} else {

		text = new sf::Text();
		text->setFont(*this->font);

		text->setCharacterSize(24);
		text->setColor(sf::Color(255, 255, 255, 255));
		SetText("");
	}

	if (s != NULL && t != 0){
		//Custom mesh?
		if (m == NULL){
			m = MeshManager::Instance().AddMesh("TLquad");
		}

		//Create Render Object
		object = new RenderObject(m, s, t);
	}
}


HUDElement HUDElement::operator=(const HUDElement& rhs){
	if (rhs.font != NULL){
		this->font = new sf::Font(*rhs.font);
	}

	if (rhs.text != NULL){
		this->text = new sf::Text(*rhs.text);
		this->text->setFont(*this->font);
	}

	if (rhs.object != NULL){
		this->object = new RenderObject(*rhs.object);
	}

	windowOffset = rhs.windowOffset;
	this->size = rhs.size;
	this->position = rhs.position;
	this->moved = rhs.moved;
	this->overlay = rhs.overlay;

	return *this;
}

/**
*	The destructor for a hud element
*/
HUDElement::~HUDElement(void)
{
	if (object != NULL){
		delete object;
	}
	if (text != NULL){
		delete text;
	}
	if (font != NULL){
		delete font;
	}
}

void HUDElement::Update(const float& msec){
	if (moved){
		if (text != NULL){
			sf::FloatRect rectangleSize = text->getLocalBounds();

			//Center the text local to our position and size. Hard coded Y correction (font padding)
			text->setPosition(position.x + (size.x / 2) - (rectangleSize.width /2), position.y + (size.y / 2) - (rectangleSize.height / 2) - 10);
		}

		if (object != NULL){
			object->SetModelMatrix(Matrix4::Translation(Vector3(position.x, position.y, z))
				* Matrix4::Scale(Vector3(size.x, size.y, 1)));
		}

		moved = false;
	}
	if (object != NULL){
		object->Update(msec);
	}
}

void HUDElement::Render(SRenderer& ortho, sf::RenderWindow& window) const{
	if (object != NULL){
		ortho.Render(*object);
	}

	if (text != NULL){
		window.pushGLStates();
		window.draw(*text);
		window.popGLStates();
	}
}
