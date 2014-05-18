#pragma once

#include "PhysicsEngine.h"
#include "PhysicsObject.h"

#include <Box2D\Box2D.h>

//Forward Declaration
class PhysicsEngine;

/**
* A class to represent a BOX2D circle.
*/
class PhysicsCircle : public PhysicsObject
{
public:
	friend class PhysicsEngine;

	virtual ~PhysicsCircle(void){ };

	virtual void Spawn();

	inline virtual float GetRadius(){ return shape.m_radius; };

	inline virtual void SetRadius(const float& radius){ shape.m_radius = radius; }

protected:
	/**
	*	Protected to prevent instantiation through somewhere other than the
	*	physics engine!
	*/
	PhysicsCircle(PhysicsEngine& pe, const float& radius, const Vector3& position = Vector3(0,0,0), bool dynamic = true);
	PhysicsCircle();

	b2CircleShape shape;
};

