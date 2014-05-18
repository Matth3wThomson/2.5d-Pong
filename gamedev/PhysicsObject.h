#pragma once

#include <Box2D\Box2D.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Common.h"

/**
* An abstract class to wrap box2d body methods for all possible subclasses.
*/
class PhysicsObject
{
public:

	PhysicsObject(void)
	{
	}

	virtual ~PhysicsObject(void)
	{
		body->GetWorld()->DestroyBody(body);	
	}

	/**
	*	Spawns the object into the physical world allowing it to
	*	interact with objects in its world
	*/
	virtual void Spawn() = 0;

	/**
	* Returns the position of the object
	*/
	virtual inline Vector3 GetPosition(){
		b2Vec2 position = body->GetWorldCenter();
		return Vector3(position.x, position.y, 0);
	}

	/**
	*	Returns the angle of the object in degrees
	*/
	virtual inline float GetAngle(){
		return body->GetAngle() * 180/PI;
	}

	/**
	*	Returns the box2D body of the object. Necessary only when
	*	wrapper methods arent adequate
	*/
	virtual inline b2Body* GetBody(){
		return this->body;
	}


	/**
	*	Sets the velocity of the object
	*/
	virtual inline void SetLinearVelocity(const Vector2& v){
		body->SetLinearVelocity(b2Vec2(v.x, v.y));
	}

	/**
	*	Returns the linear velocity of the object
	*/
	virtual Vector2 GetLinearVelocity() const{
		Vector2 temp;
		temp.x = body->GetLinearVelocity().x;
		temp.y = body->GetLinearVelocity().y;
		return temp;
	}

	/**
	*	Set the damping applied to the object
	*/
	virtual void SetLinearDamping(float ld){
		body->SetLinearDamping(ld);
	}

	/**
	*	Returns the linear damping applied to the object
	*/
	virtual float GetLinearDamping(){
		return body->GetLinearDamping();
	}

	/**
	*	Applies a force to the object
	*/
	virtual inline void ApplyForce(Vector2& v){
		body->ApplyForceToCenter(b2Vec2(v.x, v.y), true);
	}

	/**
	* Retreives from the fixtureDefinition used to create the object,
	* not the live object! Use with caution, preferably before spawning objects!
	*/
	virtual inline bool GetDynamic(){
		return bodyDef.type == b2_dynamicBody;
	}

	/**
	* Sets the restitution (elasticity) of the physics object. Uses
	* fixture definition so must be applied before spawn is called!
	*/
	virtual inline bool SetRestitution(const float& rest){
		if (fixtureDef == NULL){
			return false;
		}
		fixtureDef->restitution = rest;
		return true;
	}

	/**
	* Sets the Friction of the physics object. Uses
	* fixture definition so must be applied before spawn is called!
	*/
	virtual inline bool SetFriction(const float& fric){
		if (fixtureDef == NULL){
			return false;
		}
		fixtureDef->friction = fric;
		return true;
	}

	/**
	*	(En/Dis)ables rotation of the object
	*/
	virtual inline void SetRotationFixed(bool fixed = false){
		body->SetFixedRotation(fixed);
	}

	/**
	* Sets the Desnity of the physics object. Uses
	* fixture definition so must be applied before spawn is called!
	*/
	virtual inline bool SetDensity(float density){
		if (fixtureDef == NULL){
			return false;
		}
		fixtureDef->density = density;
		return true;
	}

	/**
	*	Translates the object to the given coordinate, and angle
	*/
	virtual inline bool SetTransform(const Vector2& v, const float& angle){
		if (fixtureDef == NULL){
			return false;
		}
		body->SetTransform(b2Vec2(v.x, v.y), angle);
		return true;
	}

	/**
	*	Sets the object to a bullet. Object has extra updates performed on it
	*	to ensure it doesnt pass through objects when travelling at high speeds
	*/
	virtual inline void SetBullet(bool bullet){
		body->SetBullet(bullet);
	}

protected:
	b2Body* body;
	b2BodyDef bodyDef;

	//Only appropriate for dynamic objects
	b2FixtureDef* fixtureDef;
};

