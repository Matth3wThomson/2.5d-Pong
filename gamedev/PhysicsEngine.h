#pragma once

#include <Box2D\Box2D.h>
#include <GL\glew.h>
#include <SFML\System\Clock.hpp>

#include <list>

#include "Vector2.h"
#include "Common.h"
#include "Matrix4.h"


#include "SRenderer.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"

#include "PhysicsContactListener.h"

//Forward Declarations
class PhysicsSquare;
#include "PhysicsSquare.h"

class PhysicsCircle;
#include "PhysicsCircle.h"

/**
* A Class that represents a BOX2D world. All object creation must be done through
* the physics engine. Can be passed a debug value of true on construction, and then
* DrawPhysics will draw the debug interface of the physics engine to the renderer supplied
*/
class PhysicsEngine
{
public:

	//Use the normal Vector2 class (hide the box2d format from the user)
	PhysicsEngine(const Vector2& gravity, bool debug = false);
	~PhysicsEngine(void);

	//Returns the box2d world
	inline b2World* GetWorld(){
		return world;
	}
	/**
	*	Uses a semi fixed time step outlined at: 
	*	http://gafferongames.com/game-physics/fix-your-timestep/
	*	This prevents render rate from affecting the number of physics updates.
	*/
	void inline Step(float timeStep = 1.0f / 60.0f, const int& velocityIterations = 8, const int& positionIterations = 3){
		//Time per frame
		float time = c.restart().asSeconds();

		if (time > timeStep){
			time = timeStep;
		}
		//Add to the accumulator
		accumulator += time;

		if (this->contactListener){
			contactListener->ClearContacted();
		}

		//If the accumulator as accumulated enough time since last frame,
		//do the step and reset accumulator
		while (accumulator >= timeStep){

			world->Step(timeStep, velocityIterations, positionIterations);
			accumulator -= timeStep;
		}
	}

	PhysicsSquare* CreatePhysicsSquare(const float& w, const float& h, const Vector3& position = Vector3(0,0,0), bool dynamic = true);
	PhysicsCircle* CreatePhysicsCircle(const float& radius, const Vector3& position = Vector3(0,0,0), bool dynamic = true);

	/**
	*	Deletes a physics object
	*/
	void DeletePhysicsObject(PhysicsObject* po){
		delete po;
	}

	/**
	*	Draws the physics using the supplied renderer. Must have been constructed
	*	in debug mode.
	*/
	void DrawPhysics(SRenderer& r);

	/**
	*	Adds the passed contact listener to the engine.
	*/
	void AddContactListener(PhysicsContactListener* contactListener){
		world->SetContactListener(contactListener);
		this->contactListener = contactListener;
	}

	/**
	*	Creates the default contact listener.
	*/
	PhysicsContactListener* CreateContactListener(){
		PhysicsContactListener* pc = new PhysicsContactListener();
		this->AddContactListener(pc);
		return pc;
	}

	/**
	*	Returns whether the two passed shapes are contacting
	*/
	bool isContacting(PhysicsObject* po, PhysicsObject* po2){
		if (contactListener != NULL){
			return contactListener->isContacting(po->GetBody(), po2->GetBody());
		}
		return false;
	}

protected:
	PhysicsEngine(void);
	void DrawSquare(b2Vec2* points, b2Vec2 center, float angle);

	//World Properties
	b2Vec2 gravity;
	b2World* world;

	//Optional contact listener
	PhysicsContactListener* contactListener;

	//Fixing the timestep
	float accumulator;
	sf::Clock c;

	//Vector to keep track of all physics squares
	vector<PhysicsSquare*> squares;

	//Vector to keep track of all physics circles
	vector<PhysicsCircle*> circles;

	//RenderObject pointers to handle rendering of physics objects
	RenderObject* sq;
	RenderObject* ci;
};

