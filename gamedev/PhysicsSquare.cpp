#include "PhysicsSquare.h"

PhysicsSquare::PhysicsSquare(PhysicsEngine& pe, const float& w, const float& h, const Vector3& position, bool dynamic){
	//Check if default constructors must be called imperatively
	if (dynamic && w <= 10.0f && h <= 10.0f){
		bodyDef.type = b2_dynamicBody;
	}

	bodyDef.position.Set(position.x, position.y);
	body = pe.GetWorld()->CreateBody(&bodyDef);

	//This takes half size, but abstracted from user!
	shape.SetAsBox(w/2, h/2);

	this->w = w;
	this->h = h;

	if (bodyDef.type == b2_dynamicBody){
		fixtureDef = new b2FixtureDef();
		fixtureDef->shape = &shape;
		fixtureDef->density = 1.0f;
		fixtureDef->friction = 0.3f;
	}
};

void PhysicsSquare::Spawn(){
	if (bodyDef.type == b2_dynamicBody){
		body->CreateFixture(fixtureDef);
	} else {
		body->CreateFixture(&shape, 0.0f);
	}
}

