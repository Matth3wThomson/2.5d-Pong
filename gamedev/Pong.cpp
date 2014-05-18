#include "Pong.h"


Pong::~Pong(void)
{

}

void Pong::LoadResources(){
	//Create the physics engine
	pe = new PhysicsEngine(Vector2(0.0f, 0.0f));

	//Creates a contact listener so collisions are recorded
	pe->CreateContactListener();

	//Its not the end of the game yet!
	gameOver = false;

	//Add all of our resources before hand.
	ResourcesManager::Instance().AddShader("basic", "testFrag.glsl", "testVert.glsl");

	ResourcesManager::Instance().AddMesh("cube3.obj");
	ResourcesManager::Instance().AddMesh("cube.obj");
	ResourcesManager::Instance().AddMesh("sphere.obj");
	ResourcesManager::Instance().AddMesh("quad");
	ResourcesManager::Instance().AddMesh("TLquad");

	ResourcesManager::Instance().AddTexture("background.jpg");
	ResourcesManager::Instance().AddTexture("gradientpurplereverse.jpg");
	ResourcesManager::Instance().AddTexture("gradientpurple.jpg");
	ResourcesManager::Instance().AddTexture("yellowandpurple.jpg");
	ResourcesManager::Instance().AddTexture("white.jpg");
	ResourcesManager::Instance().AddTexture("Transparent.png");
	ResourcesManager::Instance().AddTexture("green.jpg");
	ResourcesManager::Instance().AddTexture("lightGreen.png");
	ResourcesManager::Instance().AddTexture("darkred.jpg");

	ResourcesManager::Instance().AddSound("selected.wav");
	ResourcesManager::Instance().AddSound("wilhelm.wav");
	ResourcesManager::Instance().AddSound("blip.wav");
	ResourcesManager::Instance().AddSound("radar blip.wav");

	//Create the Bounds of play
	floor = new SquareEntity(*renderer,
		MeshManager::Instance().GetMesh("cube.obj"),
		ShaderManager::Instance().GetShader("basic"),
		*pe, 100, 20, Vector3(0, -11, 0), false,
		TextureManager::Instance().GetTexture("gradientpurplereverse.jpg"));

	ceiling = new SquareEntity(*renderer,
		MeshManager::Instance().GetMesh("cube.obj"),
		ShaderManager::Instance().GetShader("basic"),
		*pe, 100, 20, Vector3(0, 25, 0), false,
		TextureManager::Instance().GetTexture("gradientpurple.jpg"));

	leftWall = new SquareEntity(*renderer,
		MeshManager::Instance().GetMesh("cube.obj"),
		ShaderManager::Instance().GetShader("basic"),
		*pe, 5, 40, Vector3(-18, 17, 0), false,
		TextureManager::Instance().GetTexture("testTexture.jpg"));

	rightWall = new SquareEntity(*renderer,
		MeshManager::Instance().GetMesh("cube.obj"),
		ShaderManager::Instance().GetShader("basic"),
		*pe, 5, 40, Vector3(18, 17, 0), false,
		TextureManager::Instance().GetTexture("testTexture.jpg"));

	//Spawn them in the physics world!
	floor->Spawn();
	ceiling->Spawn();
	leftWall->Spawn();
	rightWall->Spawn();



	//Create the ball
	ball = new CircleEntity(*renderer,
		MeshManager::Instance().GetMesh("sphere.obj"),
		ShaderManager::Instance().GetShader("basic"),
		*pe, 1, Vector3(0, 7.5, 0), true,
		TextureManager::Instance().GetTexture("yellowandpurple.jpg"));

	//Set its properties
	ball->GetPhysicsBody()->SetRestitution(0.9f);
	ball->GetPhysicsBody()->SetDensity(0.1f);
	ball->GetPhysicsBody()->SetBullet(true);
	ball->GetPhysicsBody()->SetRadius(0.5f);
	ball->Spawn();

	minSpeed = 10;
	maxSpeed = 20;

	//Create the pongBats
	p1 = new PongBat(*pe, *renderer, 1, 3, Vector3(-13,7.5,0), 0);
	p1->Spawn();
	p1->GetPhysicsBody()->SetRotationFixed(true);

	p2 = new PongBat(*pe, *renderer, 1, 3, Vector3(13, 7.5, 0), 1);
	p2->Spawn();
	p2->GetPhysicsBody()->SetRotationFixed(true);

	//Create the midline
	midLine = new RenderObject(MeshManager::Instance().GetMesh("quad"), 
		ShaderManager::Instance().GetShader("basic"),
		TextureManager::Instance().AddTexture("white.jpg"));

	midLine->SetModelMatrix(Matrix4::Translation(Vector3(0,0,-5)) *
		Matrix4::Scale(Vector3(0.5, 40, 0)));
	renderer->AddRenderObject(*midLine);

	//Create the background image
	background = new RenderObject(MeshManager::Instance().GetMesh("quad"),
		ShaderManager::Instance().GetShader("basic"),
		TextureManager::Instance().GetTexture("background.jpg"));
	background->SetModelMatrix(Matrix4::Translation(Vector3(0,7,-10)) *
		Matrix4::Scale(Vector3(-36, -12, 0)));
	renderer->AddRenderObject(*background);

	//Create the score board
	score = new HUDElement(Vector2(200, 100),
		Vector2((float) renderer->GetWidth() / 2 - 100, (float) renderer->GetHeight() / 10 - 50),
		ShaderManager::Instance().GetShader("basic"),
		TextureManager::Instance().GetTexture("Transparent.png"),
		MeshManager::Instance().GetMesh("TLquad"), "SF Square Head.ttf");
	score->SetFontSize(72);

	//Create the resume button
	resume = new HUDElement(Vector2(300.0f, 100.0f),
		Vector2(this->hud->GetSize().x / 2, this->hud->GetSize().y / 2),
		"SF Square Head.ttf",
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("green.jpg"));
	resume->SetPosition(Vector2(this->hud->GetSize().x / 2 - (resume->GetSize().x / 2),
		this->hud->GetSize().y / 2 - (resume->GetSize().y / 2)));
	resume->SetText("Resume");
	resume->SetHighlightTexture(ResourcesManager::Instance().GetTexture("lightGreen.png"));

	//Create the quit button
	quit = new HUDElement(resume->GetSize(),
		Vector2(resume->GetPosition().x,
		resume->GetPosition().y + resume->GetSize().y + 10),
		"SF Square Head.ttf",
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("darkred.jpg"));
	quit->SetText("Quit");
	quit->SetHighlightTexture(ResourcesManager::Instance().GetTexture("lightGreen.png"));

	//Set all the appropriate attributes
	p1Score = 0;
	p2Score = 0;
	paused = false;
	gameOver = false;
	highlightedElement = 0;
	numberOfElements = 2;

	score->SetText("0    0");

	//Add score to the hud
	this->hud->AddElement(*score);
}

void Pong::DeleteResources(){
	//Delete all of the resources we created!
	delete p1, p2;
	delete ball;
	delete floor, ceiling, leftWall, rightWall;
	delete pe;

	delete midLine;
	delete background;

	delete score;
	delete resume;
	delete quit;

	//Remove all of our elements from the hud and renderer
	hud->RemoveAllElements();
	renderer->RemoveAllRenderObjects();
}

void Pong::Update(const float& time){

	if (InputHandler::Instance().IsKeyReleased(sf::Keyboard::Escape) ||
		InputHandler::Instance().IsButtonReleased(7)){
			paused = !paused;

			if (paused){
				hud->AddElement(*resume);
				hud->AddElement(*quit);

			} else {
				hud->RemoveElement(resume);
				hud->RemoveElement(quit);
			}
	}


	if (!gameOver && (p1Score >= maxScore || p2Score >= maxScore)){
		gameOver = true;
		ResourcesManager::Instance().PSound("wilhelm.wav");
	}

	if (gameOver){
		this->EndGame();

	} else if (!paused){

		this->GamePlay(time);

	} else {

		this->Paused();

	}

	hud->Update(time);
}


void Pong::RenderScreen(){
	renderer->RenderScene();
	hud->Render();
}


void Pong::GamePlay(const float& time){
	pe->Step();
	//pe->DrawPhysics(*renderer);
	if (pe->isContacting(ball->GetPhysicsBody(), p1->GetPhysicsBody())){
		ResourcesManager::Instance().PSound("blip.wav");
		Vector2 force = Vector2(5,5);
		ball->GetPhysicsBody()->ApplyForce(force);
	}

	if (pe->isContacting(ball->GetPhysicsBody(), p2->GetPhysicsBody())){
		ResourcesManager::Instance().PSound("blip.wav");
		Vector2 force = Vector2(-5,-5);
		ball->GetPhysicsBody()->ApplyForce(force);
	}

	if (pe->isContacting(ball->GetPhysicsBody(), leftWall->GetPhysicsBody())){
		p2Score++;
		std::ostringstream str;
		str << "" << p1Score << "    " << p2Score;
		score->SetText(str.str());
		ResourcesManager::Instance().PSound("radar blip.wav");
		ball->GetPhysicsBody()->SetRotationFixed(true);
		ball->GetPhysicsBody()->SetTransform(Vector2(-2, 7.5f), 0.0f);
		ball->GetPhysicsBody()->SetRotationFixed(false);
		ball->GetPhysicsBody()->SetLinearVelocity(Vector2(0.0f,0.0f));
	}

	if (pe->isContacting(ball->GetPhysicsBody(), rightWall->GetPhysicsBody())){
		p1Score++;
		std::ostringstream str;
		str << "" << p1Score << "    " << p2Score;
		score->SetText(str.str());
		ResourcesManager::Instance().PSound("radar blip.wav");
		ball->GetPhysicsBody()->SetRotationFixed(true);
		ball->GetPhysicsBody()->SetTransform(Vector2(2,7.5f), 0.0f);
		ball->GetPhysicsBody()->SetRotationFixed(false);
		ball->GetPhysicsBody()->SetLinearVelocity(Vector2(0.0f, 0.0f));
	}

	if (pe->isContacting(ball->GetPhysicsBody(), ceiling->GetPhysicsBody())){
		ball->GetPhysicsBody()->SetLinearVelocity(ball->GetPhysicsBody()->GetLinearVelocity() * 1.1f);
		ResourcesManager::Instance().PSound("blip.wav");
	}

	if (pe->isContacting(ball->GetPhysicsBody(), floor->GetPhysicsBody())){
		ball->GetPhysicsBody()->SetLinearVelocity(ball->GetPhysicsBody()->GetLinearVelocity() * 1.1f);
		ResourcesManager::Instance().PSound("blip.wav");
	}

	if (p1->GetPhysicsBody()->GetPosition().x > -1){
		p1->GetPhysicsBody()->SetTransform(Vector2(-1.0f, p1->GetPhysicsBody()->GetPosition().y), 0);
	}

	if (p2->GetPhysicsBody()->GetPosition().x < 1){
		p2->GetPhysicsBody()->SetTransform(Vector2(1.0f, p2->GetPhysicsBody()->GetPosition().y), 0);
	}

	Vector2 vel = ball->GetPhysicsBody()->GetLinearVelocity();

	//If ball is moving
	if (abs(vel.x) > 0.01f && abs(vel.y) > 0.001f){

		//If ball is moving too slow
		if (abs(vel.x) + abs(vel.y) < minSpeed){

			float factor = minSpeed / (abs(vel.x) + abs(vel.y));

			if (abs(vel.x) > abs(vel.y)){
				factor = minSpeed / abs(vel.x);
			} else {
				factor = minSpeed / abs(vel.y);
			}

			vel.x *= factor;
			vel.y *= factor;

		} //If ball is moving too fast
		else if (abs(vel.x) + abs(vel.y) > maxSpeed){

			float factor = maxSpeed / (abs(vel.x) + abs(vel.y));

			if (abs(vel.x) > abs(vel.y)){
				factor = maxSpeed / abs(vel.x);
			} else {
				factor = maxSpeed / abs(vel.y);
			}

			vel.x *= factor;
			vel.y *= factor;
		}

		ball->GetPhysicsBody()->SetLinearVelocity(vel);
	}

	ball->Update(time);
	p1->Update(time);
	p2->Update(time);
	//renderer->SetShaderLight(ball->GetPhysicsBody()->GetPosition(), Vector3(1,1,1), 50);
	renderer->UpdateScene(time);
}

void Pong::Paused(){
	if (InputHandler::Instance().MENU_MODE){
		if (InputHandler::Instance().IsKeyPressed(sf::Keyboard::Up) ||
			(InputHandler::Instance().IsAxisMoved(sf::Joystick::PovX)
			&& sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > InputHandler::Instance().JOYSTICK_TOLERANCE) ){
				highlightedElement--;
				if (highlightedElement < 0){
					highlightedElement += numberOfElements;
				}

		} else if (InputHandler::Instance().IsKeyPressed(sf::Keyboard::Down) ||
			(InputHandler::Instance().IsAxisMoved(sf::Joystick::PovX)
			&& sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -InputHandler::Instance().JOYSTICK_TOLERANCE) ){
				highlightedElement++;
		}

	} else {

		if (resume->IsMouseOver(*hud->GetWindow())){
			highlightedElement = 0;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				hud->RemoveElement(resume);
				hud->RemoveElement(quit);
				paused = false;
			}
		}

		if (quit->IsMouseOver(*hud->GetWindow())){
			highlightedElement = 1;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				ResourcesManager::Instance().PSound("selected.wav");
				EmitEvent("Menu");
			}
		}
	}

	highlightedElement %= numberOfElements;

	if (highlightedElement % numberOfElements == 0){
		resume->SetHighlighted();
		quit->SetHighlighted(false);

		if (InputHandler::Instance().IsButtonReleased(0) || InputHandler::Instance().IsKeyReleased(sf::Keyboard::Return)){
			paused = false;
			hud->RemoveElement(resume);
			hud->RemoveElement(quit);
		}
	} else {
		quit->SetHighlighted();
		resume->SetHighlighted(false);

		if (InputHandler::Instance().IsButtonReleased(0) || InputHandler::Instance().IsKeyReleased(sf::Keyboard::Return)){
			ResourcesManager::Instance().PSound("selected.wav");
			EmitEvent("Menu");
		}
	}

	if (InputHandler::Instance().IsButtonReleased(1)){
		paused = false;
		hud->RemoveElement(resume);
		hud->RemoveElement(quit);
	}
}

void Pong::EndGame(){
	if (p1Score > p2Score){
		score->SetText("P1 Wins!");
		score->SetTextColour(sf::Color(225, 45, 45, 255));
	} else {
		score->SetText("P2 Wins!");
		score->SetTextColour(sf::Color(0, 150, 255, 255));
		/*score->SetTexture(ResourcesManager::Instance().GetTexture("blue.jpg"));*/
	}

	hud->AddElement(*resume);
	hud->AddElement(*quit);

	resume->SetText("Play Again");
	quit->SetText("Quit");

	if (InputHandler::Instance().MENU_MODE){
		if (InputHandler::Instance().IsKeyPressed(sf::Keyboard::Up) ||
			(InputHandler::Instance().IsAxisMoved(sf::Joystick::PovX)
			&& sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > InputHandler::Instance().JOYSTICK_TOLERANCE) ){
				highlightedElement--;
				if (highlightedElement < 0){
					highlightedElement += numberOfElements;
				}

		} else if (InputHandler::Instance().IsKeyPressed(sf::Keyboard::Down) ||
			(InputHandler::Instance().IsAxisMoved(sf::Joystick::PovX)
			&& sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -InputHandler::Instance().JOYSTICK_TOLERANCE) ){
				highlightedElement++;
		}

	} else {

		if (resume->IsMouseOver(*hud->GetWindow())){
			highlightedElement = 0;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				ResourcesManager::Instance().PSound("selected.wav");
				EmitEvent("Pong");
			}
		}

		if (quit->IsMouseOver(*hud->GetWindow())){
			highlightedElement = 1;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				ResourcesManager::Instance().PSound("selected.wav");
				EmitEvent("Menu");
			}
		}
	}

	highlightedElement %= numberOfElements;

	if (highlightedElement % numberOfElements == 0){
		resume->SetHighlighted();
		quit->SetHighlighted(false);

		if (InputHandler::Instance().IsButtonReleased(0) || InputHandler::Instance().IsKeyReleased(sf::Keyboard::Return)){
			ResourcesManager::Instance().PSound("selected.wav");
			EmitEvent("Pong");
		}
	} else {
		quit->SetHighlighted();
		resume->SetHighlighted(false);

		if (InputHandler::Instance().IsButtonReleased(0) || InputHandler::Instance().IsKeyReleased(sf::Keyboard::Return)){
			ResourcesManager::Instance().PSound("selected.wav");
			EmitEvent("Menu");
		}
	}

	if (InputHandler::Instance().IsButtonReleased(1)){
		ResourcesManager::Instance().PSound("selected.wav");
		EmitEvent("Menu");
	}
}