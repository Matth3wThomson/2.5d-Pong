#include "TitleMenu.h"

//We set our const float here
const float TitleMenu::PADDING = 10.0f;

TitleMenu::~TitleMenu(void)
{

};

void TitleMenu::LoadResources(){
	highlightedElement = 0;
	numberOfElements = 4;

	//Add some players to our input handler using the keyboard if
	//there are no players added yet
	if (InputHandler::Instance().GetNoOfPlayers() == 0){
		InputHandler::Instance().AddPlayer(InputHandler::WASD);
		InputHandler::Instance().AddPlayer(InputHandler::ARROWKEYS);
	}

	//Add all of the resources we will use
	ResourcesManager::Instance().AddTexture("green.jpg");
	ResourcesManager::Instance().AddTexture("lightGreen.png");
	ResourcesManager::Instance().AddTexture("red.jpg");
	ResourcesManager::Instance().AddTexture("blue.jpg");
	ResourcesManager::Instance().AddTexture("darkred.jpg");
	ResourcesManager::Instance().AddTexture("unMuted.png");
	ResourcesManager::Instance().AddTexture("muted.png");
	ResourcesManager::Instance().AddTexture("backgroundReversed.jpg");
	ResourcesManager::Instance().AddTexture("TitleScreen2.png");
	ResourcesManager::Instance().AddTexture("watermark.png");

	ResourcesManager::Instance().AddSound("selected.wav");
	ResourcesManager::Instance().AddSong("MI Soundtrack.ogg");
	ResourcesManager::Instance().SetLoop("MI Soundtrack.ogg");

	//Set music going appropriately
	if (!Settings::Instance().MUTED){
		if (!ResourcesManager::Instance().GetPlaying("MI Soundtrack.ogg")){
			ResourcesManager::Instance().TPlaySong("MI Soundtrack.ogg");
		}
	} else {
		if (ResourcesManager::Instance().GetPlaying("MI Soundtrack.ogg")){
			ResourcesManager::Instance().TPlaySong("MI Soundtrack.ogg");
		}
	}

	//Create all of our elements
	this->startButton = new HUDElement(Vector2(300.0f, 100.0f),
		Vector2(this->hud->GetSize().x / 2, this->hud->GetSize().y / 2),
		"SF Square Head.ttf",
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("green.jpg"));

	startButton->SetPosition(Vector2(this->hud->GetSize().x / 2 - (startButton->GetSize().x / 2),
		this->hud->GetSize().y / 2 - (startButton->GetSize().y / 2)));
	startButton->SetText("Start");
	startButton->SetHighlightTexture(ResourcesManager::Instance().GetTexture("lightGreen.png"));


	this->playerOneScheme = new HUDElement(startButton->GetSize(),
		Vector2(startButton->GetPosition().x - (PADDING + (startButton->GetSize().x / 2)),
		startButton->GetPosition().y + startButton->GetSize().y + PADDING),
		"SF Square Head.ttf",
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("red.jpg"));
	this->playerOneScheme->SetText("P1: " + InputHandler::Instance().GetPlayerSchemeString(0));
	playerOneScheme->SetHighlightTexture(ResourcesManager::Instance().GetTexture("lightGreen.png"));

	this->playerTwoScheme = new HUDElement(startButton->GetSize(),
		Vector2(startButton->GetPosition().x + (PADDING + (startButton->GetSize().x /2)),
		startButton->GetPosition().y + startButton->GetSize().y + PADDING),
		"SF Square Head.ttf",
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("blue.jpg"));
	this->playerTwoScheme->SetText("P2: " + InputHandler::Instance().GetPlayerSchemeString(1));
	playerTwoScheme->SetHighlightTexture(ResourcesManager::Instance().GetTexture("lightGreen.png"));

	this->quitButton = new HUDElement(startButton->GetSize(),
		Vector2(startButton->GetPosition().x,
		startButton->GetPosition().y + (startButton->GetSize().y + PADDING) * 2),
		"SF Square Head.ttf",
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("darkred.jpg"));

	this->muteButton = new HUDElement(Vector2(50,50),
		Vector2(hud->GetSize().x / 10, hud->GetSize().y - 50),
		"SF Square Head.ttf",
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("unMuted.png"));
	this->muteButton->SetHighlightTexture(ResourcesManager::Instance().GetTexture("muted.png"));
	muteButton->SetHighlighted(Settings::Instance().MUTED);

	this->background = new HUDElement(this->hud->GetSize(), Vector2(0,0), 
		ShaderManager::Instance().GetShader("basic"),
		TextureManager::Instance().GetTexture("backgroundReversed.jpg"));
	this->background->SetZ(-0.5f);

	quitButton->SetText("Quit");
	quitButton->SetHighlightTexture(ResourcesManager::Instance().GetTexture("lightGreen.png"));

	title = new HUDElement(Vector2(500.0f, 200.0f),
		Vector2(this->hud->GetSize().x / 2 - 250.0f, this->hud->GetSize().y / 5 - 100.0f),
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("TitleScreen2.png"));

	watermark = new HUDElement(Vector2(300.0f, 45.0f),
		Vector2(this->hud->GetSize().x / 2 - 150.0f, this->hud->GetSize().y - 45.0f),
		ResourcesManager::Instance().GetShader("basic"),
		ResourcesManager::Instance().GetTexture("watermark.png"));

	//Add them all to our hud
	hud->AddElement(*title);
	hud->AddElement(*startButton);
	hud->AddElement(*playerOneScheme);
	hud->AddElement(*playerTwoScheme);
	hud->AddElement(*quitButton);
	hud->AddElement(*watermark);
	hud->AddElement(*muteButton);
	hud->AddElement(*this->background);
};

void TitleMenu::DeleteResources(){
	//Remove all of our elements from the hud and then delete them
	hud->RemoveAllElements();
	delete startButton;
	delete quitButton;
	delete playerOneScheme;
	delete playerTwoScheme;
	delete muteButton;
	delete title;
	delete background;
	delete watermark;
};

void TitleMenu::Update(const float& time){
	//If we are in menu mode use menu driven devices to take input
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

		//Otherwise use the mouse!
	} else {

		if (startButton->IsMouseOver(*hud->GetWindow())){
			highlightedElement = 0;


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				ResourcesManager::Instance().PSound("selected.wav");
				EmitEvent("Pong");
			}

		}

		if (playerOneScheme->IsMouseOver(*hud->GetWindow())){
			highlightedElement = 1;


			if (InputHandler::Instance().IsMouseButtonReleased()){
				InputHandler::Instance().IncrementPlayerScheme(0);
			}

		}

		if (playerTwoScheme->IsMouseOver(*hud->GetWindow())){
			highlightedElement = 2;


			if (InputHandler::Instance().IsMouseButtonReleased()){
				InputHandler::Instance().IncrementPlayerScheme(1);
			}

		}

		if (quitButton->IsMouseOver(*hud->GetWindow())){
			highlightedElement = 3;


			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				EmitEvent("Quit");
			}

		}

		if (InputHandler::Instance().IsMouseButtonReleased()){
			if (muteButton->IsMouseOver(*hud->GetWindow())){
				Settings::Instance().MUTED = !Settings::Instance().MUTED;
				muteButton->SetHighlighted(Settings::Instance().MUTED);
				ResourcesManager::Instance().TPlaySong("MI Soundtrack.ogg");
			}
		}
	}


	//Calcualte which element should be highlighted, and highlight
	highlightedElement %= numberOfElements;

	if (highlightedElement % numberOfElements == 0){ 

		startButton->SetHighlighted();
		playerOneScheme->SetHighlighted(false);
		playerTwoScheme->SetHighlighted(false);
		quitButton->SetHighlighted(false);

		//Start was pressed!
		if (InputHandler::Instance().IsButtonReleased(0)  || InputHandler::Instance().IsKeyReleased(sf::Keyboard::Return)){
			ResourcesManager::Instance().PSound("selected.wav");
			EmitEvent("Pong");
		}

	} else if (highlightedElement % numberOfElements == 1) {

		startButton->SetHighlighted(false);
		playerOneScheme->SetHighlighted();
		playerTwoScheme->SetHighlighted(false);
		quitButton->SetHighlighted(false);

		//P1 control scheme was pressed!
		if (InputHandler::Instance().IsButtonReleased(0)  || InputHandler::Instance().IsKeyReleased(sf::Keyboard::Return)){
			InputHandler::Instance().IncrementPlayerScheme(0);
			playerOneScheme->SetText("P1: " + InputHandler::Instance().GetPlayerSchemeString(0));
		}

	} else if (highlightedElement % numberOfElements == 2){

		startButton->SetHighlighted(false);
		playerOneScheme->SetHighlighted(false);
		playerTwoScheme->SetHighlighted();
		quitButton->SetHighlighted(false);

		//P2 control scheme was pressed!
		if (InputHandler::Instance().IsButtonReleased(0) || InputHandler::Instance().IsKeyReleased(sf::Keyboard::Return)){
			InputHandler::Instance().IncrementPlayerScheme(1);
			playerTwoScheme->SetText("P2: " + InputHandler::Instance().GetPlayerSchemeString(1));
		}

	} else {
		startButton->SetHighlighted(false);
		playerOneScheme->SetHighlighted(false);
		playerTwoScheme->SetHighlighted(false);
		quitButton->SetHighlighted();

		//Quit was pressed!
		if (InputHandler::Instance().IsButtonReleased(0) || InputHandler::Instance().IsKeyReleased(sf::Keyboard::Return)){
			EmitEvent("Quit");
		}
	}

	//Update our hud elements
	hud->Update(time);

	//If "B" is pressed at the title screen, just quit!
	if (InputHandler::Instance().IsButtonReleased(1)){
		EmitEvent("Quit");
	}
};

void TitleMenu::RenderScreen(){
	hud->Render();
};
