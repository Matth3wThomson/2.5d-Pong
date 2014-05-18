#pragma once

#include "RenderObject.h"
#include "PhysicsCircle.h"

/**
* A class that encapsulates a circular physics entity and a
* RenderObject. This typically represents a 2.5D physics object.
*/
class CircleEntity
{
public:


	/*
	* Constructor that takes pointers renderobject properties and physics object properties.
	* @param r The renderer this object is to be rendered in.
	* @param pe The physics engine this object is to be simulated in.
	*/ 
	CircleEntity(SRenderer& r, Mesh* m, Shader* s, PhysicsEngine& pe, const float& radius, const Vector3& position = Vector3(0,0,0), bool dynamic = true, GLuint tex = 0);

	/**
	* Constructor that takes pointers to renderobject and physics Circle
	* @param ro A pointer to the renderObject this object will use.
	* @param r The renderer this object is to be rendered in.
	* @param body Pointer to a physics Circle. This MUST have been created with the physics engine passed.
	* @param pe The physics engine this object is to be simulated in.
	*/
	CircleEntity(RenderObject* ro, SRenderer& r, PhysicsCircle* body, PhysicsEngine& pe);


	CircleEntity(void);

	/**
	* The Destructor, if its render object is not null it will delete it.
	*/
	~CircleEntity(void);

	/**
	* Creates and sets this objects physicsCircle using the properties supplied
	*/
	inline bool SetPhysicsBody(PhysicsEngine& pe, const float radius, const Vector3& position = Vector3(0,0,0), bool dynamic = true){
		body = pe.CreatePhysicsCircle(radius, position, dynamic);	
	};

	/**
	* Sets the physics body to the supplied
	*/
	inline bool SetPhysicsBody(PhysicsCircle* pc, PhysicsEngine& pe){
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
	PhysicsCircle* GetPhysicsBody(){ return body; };

	void SetRadius(const float& radius){ body->SetRadius(radius); }

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
				Matrix4::Translation(Vector3(body->GetPosition().x, body->GetPosition().y, 0.0f))
				* Matrix4::Rotation(body->GetAngle(), Vector3(0,0,1))
				* Matrix4::Scale(Vector3(body->GetRadius(), body->GetRadius(), 1)));
		}
		return true;
	};

	/**
	* Removes the RenderObject from the renderer, and deletes the physics object.
	* A new one must be supplied to re spawn this object
	*/
	bool DeSpawn(){
		pe->DeletePhysicsObject(body);
		r->RemoveRenderObject(*ro);
		body = NULL;
	}

	/**
	* Updates the Objects RenderObject according to its physical properties
	*/
	void virtual Update(float msec){
		if (body->GetDynamic()){
			ro->SetModelMatrix(
				Matrix4::Translation(Vector3(body->GetPosition().x, body->GetPosition().y, 0.0f))
				* Matrix4::Rotation(body->GetAngle(), Vector3(0,0,1))
				* Matrix4::Scale(Vector3(body->GetRadius(), body->GetRadius(), body->GetRadius())));
		}
	};

protected:

	//The render object of the entity
	RenderObject* ro;

	//The box2d wrapper for its physics entity
	PhysicsCircle* body;

	//The renderer to be rendered in
	SRenderer* r;

	//The physics engine it lives in
	PhysicsEngine* pe;
};

