#include "stdafx.h"
#include "TileMap.h"



void TileMap::clear()
{
	for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
	{
		for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
		{
			for (int z = 0; z < this->layers; z++)
			{
				for (int k = 0; k < this->map[x][y][z].size(); k++)
				{
					delete this->map[x][y][z][k];
					this->map[x][y][z][k] = NULL;
				}
				this->map[x][y][z].clear();
			}
			this->map[x][y].clear();
		}
		this->map[x].clear();
	}
	this->map.clear();

	//std::cout << this->map.size() << '\n';
}



TileMap::TileMap(float gridSize, int width, int height, std::string texture_file)
{

	this->gridSizeF = gridSize;
	this->gridSizeI = static_cast<int> (this->gridSizeF);
	this->maxSizeWorldGrid.x = width;
	this->maxSizeWorldGrid.y = height;
	this->maxSizeWorldF.x = static_cast<float> (width) * gridSize;
	this->maxSizeWorldF.y = static_cast<float> (height) * gridSize;
	this->layers = 1;
	this->textureFile = texture_file;

	this->fromX = 0;
	this->toX = 0;
	this->fromY = 0;
	this->toY = 0;
	this->layer = 0;

	this->map.resize(this->maxSizeWorldGrid.x, std::vector< std::vector < std::vector<Tile*> >>());
	for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
	{
		for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
		{
			this->map[x].resize(this->maxSizeWorldGrid.y, std::vector<std::vector<Tile*>>());

			for (int z = 0; z < this->layers; z++)
			{
				this->map[x][y].resize(this->layers, std::vector<Tile*>());
			}
		}
	}

	if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		std::cerr << "TILE::TILEMAP::INITFONTS: COULD NOT LOAD FONT";
	}

	if (!this->tileSheet.loadFromFile(texture_file))
	{
		std::cerr << "ERROR::TILEMAP::FAILED TO LOAD TILETEXTURESHEET::FILENAME: " << texture_file << '\n';
	}
	

	this->collisionBox.setSize(sf::Vector2f(gridSize, gridSize));
	this->collisionBox.setFillColor(sf::Color(255, 0, 0, 50));
	this->collisionBox.setOutlineColor(sf::Color::Red);
	this->collisionBox.setOutlineThickness(1.f);

}

TileMap::~TileMap()
{
	this->clear();
}

const bool TileMap::tileEmpty(const int x, const int y, const int z) const
{
	if (x >= 0 && x < this->maxSizeWorldF.x &&
		y >= 0 && y < this->maxSizeWorldF.y &&
		z >= 0 && z < this->layers)
	{
		return this->map[x][y][z].empty();
	}

	return false;
}

//Accesors
const sf::Texture* TileMap::getTileSheet() const
{
	return &this->tileSheet;
}

const int TileMap::getLayerSize(const int x, const int y, const int layer) const
{
	if (x >= 0 && x < static_cast<int>(this->map.size()))
	{
		if (y >= 0 && y < static_cast<int> (this->map[x].size()))
		{
			if (layer >= 0 && layer < static_cast<int> (this->map[x][y].size()))
			{
				return this->map[x][y][layer].size();
			}

		}
	}

	return -1;
}

//Functions

void TileMap::saveToFile(const std::string file_name)
{
	/*Save the entire tilemap to a text file.
	Format:
	Basic:
	Size x y
	gridsize
	texture file

	All tiles:
	gridPos x y (all tiles)
	textute rect x, y
	fill
	type
	*/
	std::ofstream out_file(file_name);

	if (out_file.is_open())
	{
		out_file << this->maxSizeWorldGrid.x << " " << this->maxSizeWorldGrid.y << '\n'
			<< this->gridSizeI << '\n'
			<< this->layers << '\n'
			<< this->textureFile << '\n';


		for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
		{
			for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
			{
				for (int z = 0; z < this->layers; z++)
				{
					if (!this->map[x][y][z].empty())
					{
						for (int k = 0; k < this->map[x][y][z].size(); k++)
						{
							out_file << x << " " << y << " " << z << " "
								<< this->map[x][y][z][k]->getAsString() << " ";

						}
					}
				}
			}
		}
	}
	else
	{
		std::cerr << "ERROR::TILEMAP::COULD NOT SAVE TO FILE::FILENAME: " << file_name << '\n';
	}

	out_file.close();
}


void TileMap::loadFromFile(const std::string file_name)
{

	std::ifstream in_file(file_name);

	if (in_file.is_open())
	{
		sf::Vector2i size;
		int gridSize = 0;
		int layers = 0;
		std::string texture_file = "";
		unsigned x = 0;
		unsigned y = 0;
		unsigned z = 0;
		unsigned trX = 0;
		unsigned trY = 0;
		bool fill = false;
		short type = 0;
		bool char_contains = false;
		std::string str = "";


		//Basics
		in_file >> size.x >> size.y >> gridSize >> layers >> texture_file;

		//Tiles
		this->gridSizeF = static_cast<float>(gridSize);
		this->gridSizeI = gridSize;
		this->maxSizeWorldGrid.x = size.x;
		this->maxSizeWorldGrid.y = size.y;
		this->layers = layers;
		this->textureFile = texture_file;

		this->clear();

		this->map.resize(this->maxSizeWorldGrid.x, std::vector< std::vector<std::vector<Tile*> >>());
		for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
		{
			for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
			{
				this->map[x].resize(this->maxSizeWorldGrid.y, std::vector < std::vector<Tile*> >());

				for (int z = 0; z < this->layers; z++)
				{
					this->map[x][y].resize(this->layers, std::vector<Tile*>());
				}
			}
		}
		if (!this->tileSheet.loadFromFile(texture_file))
			std::cerr << "ERROR::TILEMAP::FAILED TO LOAD TILETEXTURESHEET::FILENAME: " << texture_file << '\n';

		//Load all 
		
		while (in_file >> x >> y >> z >> trX >> trY >> fill >> type >> char_contains >> str)
		{

			
			this->map[x][y][z].push_back(new Tile(x, y,
				this->gridSizeF,
				this->tileSheet,
				sf::IntRect(trX, trY, this->gridSizeI, this->gridSizeI),
				this->font, str,
				fill,
				type,
				char_contains));

		}
			in_file.close();
		
	}
}

void TileMap::update()
{
}

void TileMap::render(sf::RenderTarget& target, const sf::Vector2i& gridPosition, const bool show_collision)
{

	this->layer = 0;

	this->fromX = gridPosition.x - 50;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX > this->maxSizeWorldGrid.x)
		this->fromX = this->maxSizeWorldGrid.x;

	this->toX = gridPosition.x + 50;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX > this->maxSizeWorldGrid.x)
		this->toX = this->maxSizeWorldGrid.x;

	this->fromY = gridPosition.y - 50;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->maxSizeWorldGrid.y)
		this->fromY = this->maxSizeWorldGrid.y;

	this->toY = gridPosition.y + 50;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->maxSizeWorldGrid.y)
		this->toY = this->maxSizeWorldGrid.y;

	for (int x = this->fromX; x < this->toX; x++)
	{
		for (int y = this->fromY; y < this->toY; y++)
		{
			for (int k = 0; k < this->map[x][y][this->layer].size(); k++)
			{

				if (this->map[x][y][this->layer][k]->getType() != TileTypes::DEFAULT)
				{
					switch (this->map[x][y][this->layer][k]->getType())
					{
					case TileTypes::RED:

						if (this->map[x][y][this->layer][k]->isFill())
							this->map[x][y][this->layer][k]->setFillColor(sf::Color(255, 0, 0, 90));
						else
							this->map[x][y][this->layer][k]->setOutlineColor(sf::Color::Red);
						break;
					case TileTypes::GREEN:
						if(this->map[x][y][this->layer][k]->isFill())
							this->map[x][y][this->layer][k]->setFillColor(sf::Color(0, 255, 0, 90));
						else
							this->map[x][y][this->layer][k]->setOutlineColor(sf::Color::Green);
						break;
					case TileTypes::BLUE:
						if (this->map[x][y][this->layer][k]->isFill())
							this->map[x][y][this->layer][k]->setFillColor(sf::Color(27, 42, 207, 90));
						else
							this->map[x][y][this->layer][k]->setOutlineColor(sf::Color::Blue);
						break;
					case TileTypes::YELLOW:
						if (this->map[x][y][this->layer][k]->isFill())
							this->map[x][y][this->layer][k]->setFillColor(sf::Color(255, 255, 0, 90));
						else
							this->map[x][y][this->layer][k]->setOutlineColor(sf::Color::Yellow);
						break;
					case TileTypes::WHITE:
						if (this->map[x][y][this->layer][k]->isFill())
							this->map[x][y][this->layer][k]->setFillColor(sf::Color(255, 255, 255, 90));
						else
							this->map[x][y][this->layer][k]->setOutlineColor(sf::Color::White);
						break;
					case TileTypes::ORANGE:
						if (this->map[x][y][this->layer][k]->isFill())
							this->map[x][y][this->layer][k]->setFillColor(sf::Color(255, 165, 0, 90));
						else
							this->map[x][y][this->layer][k]->setOutlineColor(sf::Color(255, 165, 0, 255));
						break;
					case TileTypes::GREY:
						if (this->map[x][y][this->layer][k]->isFill())
							this->map[x][y][this->layer][k]->setFillColor(sf::Color(128, 128, 128, 90));
						else
							this->map[x][y][this->layer][k]->setOutlineColor(sf::Color(128, 128, 128, 255));
						break;
					case TileTypes::PURPLE:
						if (this->map[x][y][this->layer][k]->isFill())
							this->map[x][y][this->layer][k]->setFillColor(sf::Color(148, 0, 211, 90));
						else
							this->map[x][y][this->layer][k]->setOutlineColor(sf::Color::Magenta);
						break;
					default:
						this->map[x][y][this->layer][k]->setOutlineColor(sf::Color::Black);
						break;
					}
				}
				
				this->map[x][y][this->layer][k]->render(target);
			}

		}
	}

}


void TileMap::addTile(const int x, const int y, const int z, const sf::IntRect& texture_rect, const bool& collision, const short& type,
	const bool& char_contains, sf::Font& font, std::string str)
{
	//Add a tile if its index fits into the size of the map

	if (x < this->maxSizeWorldGrid.x && x >= 0 &&
		y < this->maxSizeWorldGrid.y && y >= 0 &&
		z < this->layers && z >= 0)
	{

		//Can add a tile
		this->map[x][y][z].push_back(new Tile(x, y, this->gridSizeF, this->tileSheet, texture_rect, this->font, str, collision, type, char_contains));

	}
}

void TileMap::removeTile(const int x, const int y, const int z)
{
	if (x < this->maxSizeWorldGrid.x && x >= 0 &&
		y < this->maxSizeWorldGrid.y && y >= 0 &&
		z < this->layers && z >= 0)
	{
		if (!this->map[x][y][z].empty())
		{
			//Can remove a tile
			delete this->map[x][y][z][this->map[x][y][z].size() - 1];
			this->map[x][y][z].pop_back();
			/*std::cout << "DEBUG: REMOVE A TILE!" << '\n';*/
		}
	}

}

