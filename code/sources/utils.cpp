# include "includes.hpp"

void	message(std::string message)
{
	std::cout << "MESSAGE " << message << ";";
}

bool	is_tile(Tile& tile)
{
	return (tile.scrap_amount > 0);
}


bool	is_me(Tile& tile)
{
	return (is_tile(tile) && tile.owner == ME);
}

bool	is_opp(Tile& tile)
{
	return (is_tile(tile) && tile.owner == OPP);
}

bool	is_neutral(Tile& tile)
{
	return (is_tile(tile) && tile.owner == NONE);
}

bool	is_unit(Tile& tile)
{
	return (is_tile(tile) && tile.units > 0);
}

bool	is_recycler(Tile& tile)
{
	return (is_tile(tile) && tile.recycler);
}

// bool	is_recycled_by_me(Tile& tile)
// {
// 	if (is_tile(tile))
// 	{
// 		for (auto& neighbor : tile, tile.getNeighbors())
// 		{
// 			if (is_recycler(*neighbor) && is_me(*neighbor))
// 				return true;
// 		}
// 	}
// 	return false;
// }



bool	is_tile(Tile* tile)
{
	return (tile->scrap_amount > 0);
}

bool	is_usable_tile(Tile* tile)
{
	return (is_tile(tile) && !tile->recycler);
}

bool	is_my_empty_tile(Tile* tile)
{
	return (is_tile(tile) && is_usable_tile(tile) && tile->owner == ME && !tile->units);
}

bool	is_empty_opp(Tile* tile)
{
	return (is_tile(tile) && is_usable_tile(tile) && tile->owner == OPP && !tile->units);
}

