#include "SquareEntity.h"


SquareEntity::SquareEntity(void)
{
	ro = NULL;
	body = NULL;
	r = NULL;
	pe = NULL;
}

SquareEntity::SquareEntity(SRenderer& r, Mesh* m, Shader* s, PhysicsEngine& pe, const float& w, const float& h, const Vector3& position, bool dynamic, GLuint tex){
	//Create the render object for the entity
	ro = new RenderObject(m, s, tex);

	//Store a pointer to the renderer this object is to be rendered in
	this->r = &r;

	z = position.z;
	//Create a Physics Object for the entity
	body = pe.CreatePhysicsSquare(w, h, position, dynamic);

	//Store a pointer to the physics engine the object is to be simulated in
	this->pe = &pe;
};

SquareEntity::SquareEntity(RenderObject* ro, SRenderer& r, PhysicsSquare* body, PhysicsEngine& pe){
	//Store the RenderObject
	this->ro = ro;
	//Store a pointer to the Renderer this object is rendered in
	this->r = &r;

	//Store the physics Square
	this->body = body;
	//Store a pointer to the physics engine this object is to be simulated in
	this->pe = &pe;
};

SquareEntity::~SquareEntity(void)
{
	if (ro != NULL){
		delete ro;
	}

	pe->DeletePhysicsObject(this->body);

	body = NULL;
	r = NULL;
	pe = NULL;
	ro = NULL;
}
