#include "CircleEntity.h"

CircleEntity::CircleEntity(void)
{
	ro = NULL;
	body = NULL;
	r = NULL;
	pe = NULL;
}


CircleEntity::CircleEntity(SRenderer& r, Mesh* m, Shader* s, PhysicsEngine& pe, const float& radius, const Vector3& position, bool dynamic, GLuint tex){
	//Create the render object for the entity
	ro = new RenderObject(m, s, tex);
	//Store a pointer to the Renderer this object is to be rendered in
	this->r = &r;

	//Create Physics Object for the entity
	body = pe.CreatePhysicsCircle(radius, position, dynamic);

	//Store a pointer to the Physics Engine this object is to be simulated in
	this->pe = &pe;
}


CircleEntity::CircleEntity(RenderObject* ro, SRenderer& r, PhysicsCircle* body, PhysicsEngine& pe){
	//Store the RenderObject
	this->ro = ro;
	//Store a pointer to the Renderer this object is rendered in
	this->r = &r;

	//Store the physics Circle
	this->body = body;
	//Store a pointer to the physics engine this object is to be simulated in
	this->pe = &pe;
};

CircleEntity::~CircleEntity(void)
{
	//delete the render object if it has one
	if (ro != NULL){
		delete ro;
	}

	//Delete its physics entity
	pe->DeletePhysicsObject(this->body);
	body = NULL;
	r = NULL;
	pe = NULL;
}
