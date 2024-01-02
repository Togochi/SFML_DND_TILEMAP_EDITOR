#include "stdafx.h"
#include "GameState.h"


void GameState::initVariables()
{
	this->cameraSpeed = 1000.f;

}

void GameState::initKeybinds()
{

	std::ifstream ifs("Config/gamestate_keybinds.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}
	ifs.close();
}

void GameState::initDeferredRender()
{
	this->renderTexture.create(
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(sf::IntRect(0, 0,
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height));
}

void GameState::initView()
{
	this->view.setSize(sf::Vector2f(
		static_cast<float> (this->stateData->gfxSettings->resolution.width),
		static_cast<float> (this->stateData->gfxSettings->resolution.height)));
	this->view.setCenter(sf::Vector2f(
		static_cast<float> (this->stateData->gfxSettings->resolution.width) / 2.f,
		static_cast<float> (this->stateData->gfxSettings->resolution.height) / 2.f));
}

void GameState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		std::cerr << "ERROR::GAMESTATE::INITFONTS:COULD NOT LOAD FONT" << '\n';
	}
}

void GameState::initPauseMenu()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	this->pmenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font);

	this->pmenu->addButton("LOAD", gui::p2pY(46.f, vm), gui::p2pX(13.f, vm), gui::p2pY(6.f, vm), gui::calcCharSize(vm), "Load");
	this->pmenu->addButton("QUIT", gui::p2pY(74.f, vm), gui::p2pX(13.f, vm), gui::p2pY(6.f, vm), gui::calcCharSize(vm), "Quit");

}

void GameState::initTileMap(std::string file_name)
{
	file_name = file_name.substr(0, file_name.length() - 4);
	this->tileMap = new TileMap(this->stateData->gridSize, 1000, 1000, "Resources/Images/Tiles/" + file_name + ".png");
}


GameState::GameState(StateData* state_data)
	:State(state_data)
{
	this->initVariables();
	this->initDeferredRender();
	this->initView();
	this->initKeybinds();
	this->initFonts();
	this->initPauseMenu();
	this->tileMap = NULL;
}

GameState::~GameState()
{
}


void GameState::updateGameInput(const float& dt)
{
	if (!this->textbox1->getSelect())
	{
		//Move view
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_UP"))))
		{
			this->view.move(0.f, -(this->cameraSpeed * dt));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_DOWN"))))
		{
			this->view.move(0.f, this->cameraSpeed * dt);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_LEFT"))))
		{
			this->view.move(-(this->cameraSpeed * dt), 0.f);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_RIGHT"))))
		{
			this->view.move(this->cameraSpeed * dt, 0.f);
		}
	}
	
}

void GameState::updateInput(const float& dt)
{


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
	{
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
}

void GameState::updateTileMap(const float& dt)
{
	if (this->tileMap)
		this->tileMap->update();
}

void GameState::updatePauseMenuButtons()
{


	if (this->pmenu->isButtonPressed("QUIT"))
		this->endState();

	if (this->pmenu->isButtonPressed("LOAD"))
	{
		std::string file_name = this->textbox1->getText();
		this->initTileMap(file_name);
		this->tileMap->loadFromFile(file_name);
	}
	
}

void GameState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateKeytime(dt);
	this->updateInput(dt);
	this->textbox1->update(this->mousePosWindow, dt);

	if (!this->paused)
	{
		
		this->updateGameInput(dt);
		this->updateTileMap(dt);
	}
	else
	{
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}
	this->renderTexture.clear();

	this->renderTexture.setView(this->view);

	if (this->tileMap)
	{
		this->tileMap->render(this->renderTexture, this->mousePosGrid, true);
	}
		

	//Render GUI
	this->renderTexture.setView(this->renderTexture.getDefaultView());
	this->textbox1->render(this->renderTexture);

	if (this->paused)
	{
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pmenu->render(this->renderTexture);
		this->textbox1->render(this->renderTexture);
	}

	//Final render
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);

}


	
