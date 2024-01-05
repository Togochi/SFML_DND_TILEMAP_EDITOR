#include "stdafx.h"
#include "SettingsState.h"

void SettingsState::initVariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		std::cerr << "ERROR::MAINMENUSTATE::COULD NOT LOAD FONT";
	}
}

void SettingsState::initKeybinds()
{
	std::ifstream ifs("Config/mainmenustate_keybinds.ini");

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

void SettingsState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	//Background
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height)
		)
	);

	if (!this->backgroundTexture.loadFromFile("Resources/Images/Backgrounds/bg2.jpg"))
	{
		std::cerr << "ERROR::MAINMENUSTATE::COULD NOT LOAD FONT";
	}

	this->background.setTexture(&this->backgroundTexture);

	//Buttons
	this->buttons["BACK"] = new gui::Button(
		gui::p2pX(72.f, vm), gui::p2pY(81.5f, vm),
		gui::p2pX(13.f, vm), gui::p2pY(6.f, vm),
		&this->font, "Back", gui::calcCharSize(vm),
		sf::Color(236, 239, 241, 200), sf::Color(236, 239, 241, 250), sf::Color(236, 239, 241, 50),
		sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	//this->buttons["APPLY"] = new gui::Button(
	//	gui::p2pX(60.f, vm), gui::p2pY(81.5f, vm),
	//	gui::p2pX(13.f, vm), gui::p2pY(6.f, vm),
	//	&this->font, "Apply", gui::calcCharSize(vm),
	//	sf::Color(70, 70, 70, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
	//	sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	//Modes
	std::vector<std::string> modes_str;
	for (auto &i : this->modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}


	//Text init
	this->optionsText.setFont(this->font);
	this->optionsText.setPosition(sf::Vector2f(10.f, 10.f));
	this->optionsText.setCharacterSize(18);
	this->optionsText.setFillColor(sf::Color(238, 238, 238, 255));

	this->optionsText.setString(
		"To configure the graphics, go to Config/graphics.ini and go to the settings: " 
		"\n - title \n - resolution.width / resolution.height\n - Full screen mode(0 - off, 1 - on)"
		"\n - Frame rate limitation\n - vertical synchronization(0 - off, 1 - on)\n - Grid size (32, 64, or any other)"
		"\n\nTo enter text in game mode, use the button in the upper left corner of the screen.\n In editor mode, this option is available only in the menu (press Escape)."
		"\nTo delete all the entered text, you can press Tab."
		"\n\nTo save or download any file, first call the text input mode. " 
		"\nEnter the full name of the file you are uploading or creating and use the mouse to select the desired command in the menu (press Escape)."
		"\nExample: test1.txt"
		""
	);
}

void SettingsState::resetGui()
{
	/*
	 * Clears the GUI elements and re-initialises the GUI.
	 *
	 * @return void
	 */

	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	this->buttons.clear();

	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
	this->dropDownLists.clear();

	this->initGui();
}

SettingsState::SettingsState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
}

SettingsState::~SettingsState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	
	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
}

//Accessors

//Functions
void SettingsState::updateInput(const float & dt)
{

}

void SettingsState::updateGui(const float & dt)
{
	/*Updates all the gui elements in the state and handle their functionlaity.*/
	//Buttons
	for (auto &it : this->buttons)
	{
		it.second->update(this->mousePosWindow, dt);
	}

	//Button functionality
	//Quit the game
	if (this->buttons["BACK"]->isPressed())
	{
		this->endState();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
	{
		this->endState();
	}
	////Apply selected settings
	//if (this->buttons["APPLY"]->isPressed())
	//{
	//
	//	this->stateData->gfxSettings->resolution = this->modes[this->dropDownLists["RESOLUTION"]->getActiveElementId()];
	//}

	//Dropdown lists functionality
}

void SettingsState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateKeytime(dt);
	this->updateInput(dt);

	this->updateGui(dt);
}

void SettingsState::renderGui(sf::RenderTarget& target)
{
	for (auto &it : this->buttons)
	{
		it.second->render(target);
	}

}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	this->renderGui(*target);

	target->draw(this->optionsText);

}