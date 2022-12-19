# include "includes.hpp"

void	message(std::string message)
{
	std::cout << "MESSAGE " << message << ";";
}

bool	is_tile(Tile* tile)
{
	return (tile && tile->scrap_amount > 0);
}

bool	is_usable_tile(Tile* tile)
{
	return (is_tile(tile) && tile->recycler);
}

bool	is_my_empty_tile(Tile* tile)
{
	return (is_tile(tile) && is_usable_tile(tile) && tile->owner == ME && !tile->units);
}
