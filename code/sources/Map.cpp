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
	for	(std::vector<Tile> &row: _tiles)
	{
		for (Tile &tile : row)
			tile.read();
	}
}

Tile &	Map::tileAt(int x, int y)
{
	return _tiles[x][y];
}


Tile &	Map::center()
{
	return tileAt(_width / 2, _height / 2);
}

const Tile &	Map::closestEnemy( const Tile& to ) const
{
	Tile const *closest;
	int	min = 999;
	for (auto& row : _tiles)
	{
		for (auto& tile : row)
		{
			if (tile.isEnemy() && tile._units && tile.distance(to) < min)
			{
				min = tile.distance(to);
				closest = &tile;
				debug("min = ", min);
			}
		}
	}
	debug("closest : ", *closest);
	return (*closest);
}


int		Map::_id(const Tile& tile) const
{
	return (tile._row * _width + tile._col);
}
