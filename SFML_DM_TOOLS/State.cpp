#include "stdafx.h"
#include "State.h"

State::State(StateData* state_data)
{
	this->stateData = state_data;
	this->window = state_data->window;
	this->supportedKeys = state_data->supportedKeys;
	this->states = state_data->states;
	this->quit = false;
	this->paused = false;
	this->keytime = 0.f;
	this->keytimeMax = 10.f;
	this->gridSize = state_data->gridSize;

	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		throw ("ERROR::MAINMENUSTATE::INITFONTS:COULD NOT LOAD FONT");
	}

	this->textbox1 = new gui::TextBox(16, sf::Color(238, 238, 238, 255), false, 0.f, 0.f, this->font);
	this->textbox1->setFont(this->font);
	
	this->textbox1->setPosition(sf::Vector2f(10.f, 80.f));

	std::cout << "TEXTBOX CREATE!" << '\n';
	
}

State::~State()
{
	delete this->textbox1;
	std::cout << "TEXTBOX DELETE!" << '\n';
}

//Accessors
const bool & State::getQuit() const
{
	return this->quit;
}

const bool State::getKeytime()
{
	if (this->keytime >= this->keytimeMax)
	{
		this->keytime = 0.f;
		return true;
	}

	return false;
}

//Functions
void State::endState()
{
	this->quit = true;
}

void State::pauseState()
{
	this->paused = true;
}

void State::unpauseState()
{
	this->paused = false;
}

void State::callingTextInput(sf::Event input)
{
	this->textbox1->typedOn(input);
}

void State::updateMousePositions(sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

	if(view)
		this->window->setView(*view);

	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
	this->mousePosGrid = 
		sf::Vector2i(
			static_cast<int>(this->mousePosView.x) / static_cast<int>(this->gridSize),
			static_cast<int>(this->mousePosView.y) / static_cast<int>(this->gridSize)
		);
	
	this->window->setView(this->window->getDefaultView());
}

void State::updateKeytime(const float& dt)
{
	if (this->keytime < this->keytimeMax)
		this->keytime += 100.f * dt;
}
