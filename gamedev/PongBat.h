#pragma once

#include "SquareEntity.h"
#include "MeshManager.h"
#include "ResourcesManager.h"
#include "InputHandler.h"
#include "Settings.h"

/**
* A class to represent a pong bat. An example of how the engine should be used.
*/
class PongBat : public SquareEntity
{
public:

	/**
	* The constructor for a pong bat, calls the required parent constructor
	*/
	PongBat(PhysicsEngine& pe, SRenderer& r, const float& w, const float& h, const Vector3& position,const int& player) 
		: SquareEntity(r,
		MeshManager::Instance().AddMesh("cube3.obj"),
		ShaderManager::Instance().GetShader("basic"),
		pe, w, h, position, true,
		TextureManager::Instance().AddTexture("red.jpg")){

			if (player == 1){ this->GetRenderObject()->SetTexture(TextureManager::Instance().AddTexture("blue.jpg")); }
			this->body->SetRotationFixed(true);
			this->body->SetDensity(1000.0f);
			this->player = player;
	};

	virtual ~PongBat(void){ };

	/**
	* Calls the parent update. This is an example of how control schemes will
	* be implemented. However, the input handler class will be used to manage
	* which control scheme should be used to modify each object.
	*/
	void virtual Update(float msec){
		SquareEntity::Update(msec);

		float x = 0, y = 0, z = 0;

		//Retreive what input scheme our player is using.
		InputHandler::inputScheme is = InputHandler::Instance().GetPlayerScheme(this->player);

		//Act accordingly
		switch (is){
		case InputHandler::WASD:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
				y -= 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
				y += 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
				x -= 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				x += 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)){
				z -= 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
				z += 100;
			}
			break;
		case InputHandler::ARROWKEYS:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
				y -= 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
				y += 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
				x -= 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
				x += 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)){
				z -= 100;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)){
				z += 100;
			}
			break;
		default:
			//Must be a controller!
			y = sf::Joystick::getAxisPosition(is, sf::Joystick::Y);
			x = sf::Joystick::getAxisPosition(is, sf::Joystick::X);
			z = sf::Joystick::getAxisPosition(is, sf::Joystick::Z);
			break;
		}

		//Modifications for joystick tolerance
		if (abs(y) < InputHandler::Instance().JOYSTICK_TOLERANCE){
			y = 0;
		}

		if (abs(x) < InputHandler::Instance().JOYSTICK_TOLERANCE){
			x = 0;
		}

		//Transformations for rotation
		z = (float) DegToRad(z);
		z /= 3;

		//Transformations for movement
		x *=  0.16f;
		y *= 0.1f;

		//Set the bodies appropriate values
		this->body->SetTransform(Vector2(this->body->GetPosition().x, this->body->GetPosition().y), z);
		this->body->SetLinearVelocity(Vector2(x, -y));

	}


protected:

	//To keep track of which player this object represents
	int player;
};

