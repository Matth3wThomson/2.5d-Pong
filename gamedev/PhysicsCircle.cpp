#include "PhysicsCircle.h"


PhysicsCircle::PhysicsCircle(PhysicsEngine& pe, const float& radius, const Vector3& position, bool dynamic)
{
	if (dynamic && radius <= 5.0f){
		bodyDef.type = b2_dynamicBody;
	}

	bodyDef.position.Set(position.x, position.y);
	body = pe.GetWorld()->CreateBody(&bodyDef);

	shape.m_p.Set(0,0);
	shape.m_radius = radius;

	if (bodyDef.type == b2_dynamicBody){
		fixtureDef = new b2FixtureDef();
		fixtureDef->shape = &shape;
		fixtureDef->density = 1.0f;
		fixtureDef->friction = 0.3f;
	}
}

void PhysicsCircle::Spawn(){
	if (bodyDef.type == b2_dynamicBody){
		body->CreateFixture(fixtureDef);
	} else {
		body->CreateFixture(&shape, 0.0f);
	}
}
