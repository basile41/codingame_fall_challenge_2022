#include "includes.hpp"
#include "Data.hpp"

// Data::Data(int width, int height): width(width), height(height), tiles(width * height)
// {
// }

void Data::read()
{
	int id = 0;
	// tiles.reserve(width * height);
	std::cin >> my_matter >> opp_matter;
	std::cin.ignore();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++, id++)
		{
			int scrap_amount;
			int owner; // 1 = me, 0 = foe, -1 = neutral
			int units;
			int recycler;
			int can_build;
			int can_spawn;
			int in_range_of_recycler;
			std::cin >> scrap_amount >> owner >> units >> recycler >> can_build >> can_spawn >> in_range_of_recycler;
			std::cin.ignore();

			Tile tile = {id, x, y, scrap_amount, owner, units,
						 recycler == 1, can_build == 1, can_spawn == 1, in_range_of_recycler == 1};
			tiles.push_back(tile);

			Tile* tmp = getTile(x, y);
			if (tile.owner == ME)
			{
				my_tiles.push_back(tmp);
				if (tile.units > 0)
				{
					my_units.push_back(tmp);
				}
				else if (tile.recycler)
				{
					my_recyclers.push_back(tmp);
				}
			}
			else if (tile.owner == OPP)
			{
				opp_tiles.push_back(tmp);
				if (tile.units > 0)
				{
					opp_units.push_back(tmp);
				}
				else if (tile.recycler)
				{
					opp_recyclers.push_back(tmp);
				}
			}
			else
			{
				neutral_tiles.push_back(tmp);
			}
		}
	}
	
}


Tile *	Data::getTile(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return NULL;
	return (&tiles[y * width + x]);
}


Tile *	Data::getValidTile(int x, int y)
{
	Tile* tile = getTile(x, y);
	if (!tile || tile->scrap_amount == 0)
		return NULL;
	return tile;
}

Tile *	Data::getUsableTile(int x, int y)
{
	Tile* tile = getValidTile(x, y);
	if (!tile || tile->recycler)
		return NULL;
	return tile;
}

