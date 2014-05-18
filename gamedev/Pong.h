#pragma once

#include "Screen.h"
#include "ResourcesManager.h"
#include "SquareEntity.h"
#include "CircleEntity.h"
#include "PongBat.h"
#include "PhysicsContactListener.h"

#include <sstream>
#include <string>

/**
*	This class represents an example of how the engine should
*	be used. It is our pong game.
*/
class Pong : public Screen
{
public:

	//The constructor for a pong game.
	Pong(SRenderer* proj, HUD* hud, int maxScore) : Screen(hud, proj){
		this->maxScore = maxScore;		
	};

	virtual ~Pong(void);

	virtual void LoadResources();
	virtual void DeleteResources();
	virtual void Update(const float& time);
	virtual void RenderScreen();


	virtual void ReceiveEvent(const string& event){ };

	/**
	*	A method used to handle all unpaused game play updating
	*/
	void GamePlay(const float& time);

	/**
	*	A method used to handle all logic whilst the game is paused
	*/
	void Paused();

	/**
	*	A method used to handle all logic when the game is finished
	*/
	void EndGame();



protected:

	//All of our objects
	PhysicsEngine* pe;
	SquareEntity* floor, * ceiling, * leftWall, * rightWall;
	CircleEntity* ball;
	PongBat* p1, * p2;

	RenderObject* midLine;
	RenderObject* background;

	HUDElement* score;
	HUDElement* resume;
	HUDElement* quit;

	//Float values used to cap the speed of the ball
	float maxSpeed;
	float minSpeed;

	//The values representing the scores of each player and the max
	int p1Score, p2Score, maxScore;

	//The values used to enable menu driven operation
	int highlightedElement;
	int numberOfElements;

	//Bools used to represent game state.
	bool gameOver;
	bool paused;
};

