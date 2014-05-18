#pragma once

#include "RenderObject.h"
#include "PhysicsSquare.h"

/**
* A class that encapsulates a square physics entity and a
* RenderObject.
*/
class SquareEntity
{
public:

	/*
	* Constructor that takes pointers renderobject properties and physics object properties.
	* @param r The renderer this object is to be rendered in.
	* @param pe The physics engine this object is to be simulated in.
	*/ 
	SquareEntity(SRenderer& r, Mesh* m, Shader* s, PhysicsEngine& pe, const float& w, const float& h, const Vector3& position = Vector3(0,0,0), bool dynamic = true, GLuint tex = 0);

	/**
	* Constructor that takes pointers to renderobject and physics Square
	* @param ro A pointer to the renderObject this object will use.
	* @param r The renderer this object is to be rendered in.
	* @param body Pointer to a physics Square. This MUST have been created with the physics engine passed.
	* @param pe The physics engine this object is to be simulated in.
	*/
	SquareEntity(RenderObject* ro, SRenderer& r, PhysicsSquare* body, PhysicsEngine& pe);

	SquareEntity(void);

	/**
	* The Destructor, if its render object is not null it will delete it.
	*/
	virtual ~SquareEntity(void);

	/**
	* Creates and sets this objects physicsCircle using the properties supplied
	*/
	inline bool SetPhysicsBody(PhysicsEngine& pe, const float& w, const float& h, const Vector3& position = Vector3(0,0,0), bool dynamic = true){
		body = pe.CreatePhysicsSquare(w, h, position, dynamic);
	};

	/**
	* Sets the physics body to the supplied
	*/
	inline bool SetPhysicsBody(PhysicsSquare* pc, PhysicsEngine& pe){
		body = pc;
		this->pe = &pe;
	};

	/**
	* Creates and sets this objects RenderObject using the properties supplied
	*/
	inline bool SetRenderObject(SRenderer& r, Mesh*m, Shader*s, GLuint t = 0){
		ro = new RenderObject(m, s, t);
		this->r = &r;
	};

	/**
	* Sets the Render object to the one supplied
	*/
	bool SetRenderObject(SRenderer& r, RenderObject* ro){
		this->ro = ro;
		this->r = &r;
	};

	/**
	* Returns a pointer to the renderObject this object contains
	*/
	inline RenderObject* GetRenderObject(){ return ro; };
	PhysicsSquare* GetPhysicsBody(){ return body; };

	/**
	* Adds the physics object to the physics world, and the renderObject to the renderer
	*/
	bool Spawn(){
		if (body == NULL || ro == NULL){ 
			return false;
		}

		body->Spawn();
		r->AddRenderObject(*ro);
		if (!body->GetDynamic()){
			ro->SetModelMatrix(
				Matrix4::Translation(Vector3(body->GetPosition().x, body->GetPosition().y, z))
				* Matrix4::Rotation(body->GetAngle(), Vector3(0,0,1))
				* Matrix4::Scale(Vector3(body->GetWidth() * 0.5f, body->GetHeight() * 0.5f, 1)));
		}
		return true;
	};

	/**
	* Updates the Objects RenderObject according to its physical properties
	*/
	void virtual Update(float msec){
		if (body->GetDynamic()){
			ro->SetModelMatrix(
				Matrix4::Translation(Vector3(body->GetPosition().x, body->GetPosition().y, z))
				* Matrix4::Rotation(body->GetAngle(), Vector3(0,0,1))
				* Matrix4::Scale(Vector3(body->GetWidth() * 0.5f, body->GetHeight() * 0.5f, 1)));
		}
	};

protected:
	//The render object representing this 2.5d object
	RenderObject* ro;

	//The physics wrapper representing this 2.5d object in the world
	PhysicsSquare* body;

	//The renderer this object is to be rendered in
	SRenderer* r;

	//The physics engine this object is to act within
	PhysicsEngine* pe;

	//The z coordinate of the object. Should not be modified,
	//as representations of the physics will look incorrect due to
	//perspective division
	float z;
};

