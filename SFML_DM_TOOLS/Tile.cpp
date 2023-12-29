#include "stdafx.h"
#include "Tile.h"

Tile::Tile()
{
	this->fill = false;
	this->type = 0;
}

Tile::Tile(int grid_x, int grid_y, float gridSizeF, const sf::Texture& texture, const sf::IntRect& texture_rect,
	bool fill, short type)
{
	this->shape.setPosition(static_cast<float> (grid_x) * gridSizeF, static_cast<float> (grid_y) * gridSizeF);
	this->shape.setTexture(texture);
	this->shape.setTextureRect(texture_rect);

	this->contour.setSize(sf::Vector2f(shape.getGlobalBounds().width, shape.getGlobalBounds().height));
	this->contour.setPosition(shape.getPosition());
	this->contour.setFillColor(sf::Color::Transparent);
	this->contour.setOutlineColor(sf::Color::Black);
	this->contour.setOutlineThickness(-1.f);

	this->fill = fill;
	this->type = type;
}

Tile::~Tile()
{

}

const short& Tile::getType() const
{
	return this->type;
}

const bool& Tile::isFill() const
{
	return this->fill;
}

const sf::Vector2f& Tile::getPosition() const
{
	return this->shape.getPosition();
}

const sf::FloatRect Tile::getGlobalBounds() const
{
	return this->shape.getGlobalBounds();
}

const bool Tile::intersects(const sf::FloatRect bounds) const
{
	return this->shape.getGlobalBounds().intersects(bounds);
}

const std::string Tile::getAsString() const
{
	std::stringstream ss;

	ss << this->shape.getTextureRect().left << " " << this->shape.getTextureRect().top << " " << this->fill << " " << this->type;

	return ss.str();
}

void Tile::setOutlineColor(const sf::Color color)
{
	this->contour.setOutlineColor(color);
}

void Tile::setFillColor(const sf::Color color)
{
	this->contour.setFillColor(color);
}

void Tile::update()
{

}

void Tile::render(sf::RenderTarget& target)
{
		target.draw(this->shape);
		target.draw(this->contour);

}