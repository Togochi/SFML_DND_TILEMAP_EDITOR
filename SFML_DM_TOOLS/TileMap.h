#ifndef TILEMAP_H
#define TILEMAP_H

#include "Tile.h"


class Tile;

class TileMap
{
private:
	void clear();

	float gridSizeF;
	int gridSizeI;
	int layers;
	sf::Vector2i maxSizeWorldGrid;
	sf::Vector2f maxSizeWorldF;
	std::vector<std::vector < std::vector < std::vector <Tile*> > > > map;
	std::string textureFile;
	sf::Texture tileSheet;

	std::stack<Tile*> deferredRenderStack;
	sf::RectangleShape collisionBox;

	//Culling
	int fromX;
	int toX;
	int fromY;
	int toY;
	int layer;

public:
	TileMap(float gridSize, int width, int height, std::string texture_file);
	virtual ~TileMap();

	//Accessors
	const bool tileEmpty(const int x, const int y, const int z) const;
	const sf::Texture* getTileSheet() const;
	const int getLayerSize(const int x, const int y, const int z) const;

	//Function

	void saveToFile(const std::string file_name);
	void loadFromFile(const std::string file_name);

	void update();

	void render(sf::RenderTarget& target, const sf::Vector2i& gridPosition,
		const bool show_collision = false);

	void addTile(const int x, const int y, const int z, const sf::IntRect& texture_rect, const bool& fill, const short& type);
	void removeTile(const int x, const int y, const int z);

};



#endif