#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "PauseMenu.h"
#include "TileMap.h"

class GameState :
    public State
{
private:
    sf::View view;
    sf::RenderTexture renderTexture;
    sf::Sprite renderSprite;

    sf::Font font;

    PauseMenu* pmenu;
    TileMap* tileMap;

    float cameraSpeed;

    //Functions
    void initVariables();
    void initKeybinds();
    void initDeferredRender();
    void initView();
    //void initTexture();
    void initFonts();
    void initPauseMenu();
    void initTileMap(std::string file_name);
public:
    GameState(StateData* state_data);
    ~GameState();


    //Functions
    void updateInput(const float& dt);
    void updateGameInput(const float& dt);
    void updatePauseMenuButtons();
   
    void updateTileMap(const float& dt);
    void update(const float& dt);

    void render(sf::RenderTarget* target = nullptr);
};

#endif // !GAMESTATE_H