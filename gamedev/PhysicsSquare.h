#pragma once

#include "PhysicsEngine.h"
#include "PhysicsObject.h"
#include "Vector3.h"
#include "Common.h"

#include <Box2D\Box2D.h>

//Forward Declaration
class PhysicsEngine;

/**
*	Class that represents a square in the physics world
*/
class PhysicsSquare : public PhysicsObject
{
public:
	friend class PhysicsEngine;

	virtual ~PhysicsSquare(void){ };

	virtual void Spawn();

	inline float GetWidth(){ return w; };
	inline float GetHeight(){ return h; };

protected:

	/**
	* Protected constructor. Objects must be created through the physics engine!
	*/
	PhysicsSquare(PhysicsEngine& pe, const float& w, const float& h, const Vector3& position = Vector3(0,0,0), bool dynamic = true);
	PhysicsSquare(void);

	b2PolygonShape shape;

	float w, h;

};

