#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "PauseMenu.h"
#include "TileMap.h"


class GameState :
    public State
{
private:
    //Variables
    sf::View view;

    sf::Font font;
    sf::Text cursorText;
    PauseMenu* pmenu;
    TileMap* tileMap;

    std::map <std::string, gui::Button*> buttons;
    sf::RectangleShape selectorRect;
    sf::IntRect textureRect;
    gui::TextureSelector* textureSelector;
    sf::RectangleShape sidebar;

    float cameraSpeed;
    bool collision;
    short type;
    int layer;

    //Functions
    void initVariables();
    void initView();
    void initBackground();
    void initFonts();
    void initText();
    void initKeybinds();
    void initPauseMenu();
    void initGui();
    void initButtons();
    void initTileMap();
public:
    GameState(StateData* state_data);
    virtual ~GameState();

    //Functions
    void updateInput(const float& dt);
    void upateEditorInput(const float& dt);
    void updateButtons();
    void updateGui(const float& dt);
    void updatePauseMenuButtons();
    void update(const float& dt);

    void render(sf::RenderTarget* target = nullptr);
    void renderButtons(sf::RenderTarget& target);
    void renderGui(sf::RenderTarget& target);

};

#endif // !GAMESTATE_H