# include "Map.hpp"

Map::Map(int width, int height) : _tiles(height), _width(width), _height(height)
{
	for (int row = 0; row < _height; row++)
	{
		for (int col = 0; col < _width; col++)
		{
			_tiles[row].push_back(Tile(col, row));
			if (row > 0)
			{
				_tiles[row][col].setUp(&_tiles[row - 1][col]);
				_tiles[row - 1][col].setDown(&_tiles[row][col]);
			}
			if (col > 0)
			{
				_tiles[row][col - 1].setLeft(&_tiles[row][col]);
				_tiles[row][col].setRight(&_tiles[row][col - 1]);
			}

		}
	}
}

void	Map::read()
{
	std::cin >> _my_matter >> _opp_matter; std::cin.ignore();
	_my_tiles.clear();
	_opp_tiles.clear();
	for	(std::vector<Tile> &row: _tiles)
	{
		for (Tile &tile : row)
		{
			tile.read();
			if (tile.isEnemy())
				_opp_tiles.push_back(&tile);
			if (tile.isMe())
				_my_tiles.push_back(&tile);
		}
	}
}


Tile *	Map::center()
{
	return &_tiles[_height / 2][_width / 2];
}

Tile *	Map::closestEnemy( Tile& to )
{
	Tile	*closest;
	int	min = 999;
	for (auto& tile : _opp_tiles)
	{
		if (tile->_units && tile->distance(to) < min)
		{
			min = tile->distance(to);
			closest = tile;
		}
	}
	return (closest);
}



Tile *	Map::getTile(int x, int y)
{
	return &_tiles[x][y];
}
