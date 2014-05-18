#include "PhysicsEngine.h"


PhysicsEngine::PhysicsEngine(const Vector2& gravity, bool debug){

	b2Vec2 grav(gravity.x, gravity.y);
	this->gravity = grav;
	this->contactListener = NULL;
	this->sq = NULL;
	this->ci = NULL;

	world = new b2World(this->gravity);

	//If in debug mode creates render objects to draw the objects we have defined!
	if (debug){
		Shader* s = ShaderManager::Instance().GetShader("basic");
		if (s == NULL){
			s = ShaderManager::Instance().AddShader("basic", "testVert.glsl", "testFrag.glsl");
		}

		sq = new RenderObject(MeshManager::Instance().AddMesh("quad"), s, TextureManager::Instance().AddTexture("testTexture.jpg"));
		ci = new RenderObject(MeshManager::Instance().AddMesh("circle"), s, TextureManager::Instance().GetTexture("testTexture.jpg"));
	}

	accumulator = 0;
}


PhysicsEngine::~PhysicsEngine(void)
{
	//This will delete the world along with all objects within it!
	delete world;

	if (sq != NULL){
		delete sq;
		delete ci;
	}
}

void PhysicsEngine::DrawPhysics(SRenderer& r){

	//Make sure debug mode was set!
	if (sq != NULL){

		//Render all the squares
		for (vector<PhysicsSquare*>::iterator i = squares.begin(); i != squares.end(); ++i){
			sq->SetModelMatrix(
				Matrix4::Translation(Vector3((*i)->GetPosition().x, (*i)->GetPosition().y, -0.1f))
				* Matrix4::Rotation((*i)->GetAngle(), Vector3(0,0,1))
				* Matrix4::Scale(Vector3((*i)->GetWidth() * 0.5f, (*i)->GetHeight() * 0.5f, 1)));
			//Ghost update just to set world transform
			sq->Update(0.0f);
			r.Render(*sq);
		}

		//Render all the circles
		for (vector<PhysicsCircle*>::iterator i = circles.begin(); i != circles.end(); ++i){
			ci->SetModelMatrix(Matrix4::Translation(Vector3((*i)->GetPosition().x, (*i)->GetPosition().y, -0.1f))
				* Matrix4::Scale(Vector3((*i)->GetRadius() * 2, (*i)->GetRadius() * 2, 1))
				* Matrix4::Rotation((*i)->GetAngle(), Vector3(0,0,1)));
			//Ghost update just to set world transform
			ci->Update(0.0f);
			r.Render(*ci);
		}
	}
}

//Creation of physics square
PhysicsSquare* PhysicsEngine::CreatePhysicsSquare(const float& w, const float& h, const Vector3& position, bool dynamic){
	PhysicsSquare* ps = new PhysicsSquare(*this, w, h, position, dynamic);
	squares.push_back(ps);

	return ps;
}

//Creation of physics circle
PhysicsCircle* PhysicsEngine::CreatePhysicsCircle(const float& radius, const Vector3& position, bool dynamic){
	PhysicsCircle* pc = new PhysicsCircle(*this, radius, position, dynamic);
	circles.push_back(pc);

	return pc;
}
