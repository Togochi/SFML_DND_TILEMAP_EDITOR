#ifndef TILE_H
#define TILE_H

#pragma once

enum TileTypes { DEFAULT = 0, RED, GREEN, BLUE, YELLOW, WHITE, GREY, ORANGE, PURPLE};

class Tile
{
private:

protected:
	sf::Sprite shape;
	sf::RectangleShape contour;
	bool fill;
	bool charContains;
	short type;

	sf::Text text;
	sf::Font font;
	const std::string str;

public:
	Tile();
	Tile(int grid_x, int grid_y, float gridSizeF, const sf::Texture& texture, const sf::IntRect& texture_rect,
		sf::Font& font, std::string str,
		bool fill = false, short type = TileTypes::DEFAULT, bool char_contains = false);
	virtual ~Tile();

	//Accessors
	const short& getType() const;
	const bool& isFill() const;
	const bool& isCharContains() const;
	

	const sf::Vector2f& getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const std::string getAsString() const;
	const std::string getText(const std::string str) const;

	//Modifiers
	void setOutlineColor(const sf::Color color);
	void setFillColor(const sf::Color color);

	//Functions
	const bool intersects(const sf::FloatRect bounds) const;
	void update();
	void render(sf::RenderTarget& target);
};



#endif