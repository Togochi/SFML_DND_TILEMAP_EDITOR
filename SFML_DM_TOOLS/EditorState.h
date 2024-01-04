#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "State.h"
#include "PauseMenu.h"
#include "TileMap.h"


class EditorState :
    public State
{
private:
    //Variables


    sf::View view;

    sf::Font font;
    sf::Text cursorText;
    sf::Text nullText;
    PauseMenu* pmenu;
    TileMap* tileMap;

    std::map <std::string, gui::Button*> buttons;
    sf::RectangleShape selectorRect;
    sf::IntRect textureRect;
    gui::TextureSelector* textureSelector;
    sf::RectangleShape sidebar;

    float cameraSpeed;
    bool fill;
    short type;
    int layer;
    bool tileAddLock;
    bool charContains;
    bool showText;


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
    void initTileMap(std::string file_name);
    void initTileGui();
public:
    EditorState(StateData* state_data);
     ~EditorState();

     //Accessors
     std::string enumToString(short type);

    //Functions

    void updateInput(const float& dt);
    void upateEditorInput(const float& dt);
    void updateButtons(const float& dt);
    void updateGui(const float& dt);
    void updatePauseMenuButtons();
    void update(const float& dt);

    void render(sf::RenderTarget* target = NULL);
    void renderButtons(sf::RenderTarget& target);
    void renderGui(sf::RenderTarget& target);

};

#endif

