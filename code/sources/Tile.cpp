#include "includes.hpp"
#include "Tile.hpp"


Tile::Tile(size_t nb_of_tiles)
: left(nullptr), right(nullptr), top(nullptr), bottom(nullptr), distances(nb_of_tiles, 999), isolated(false)
{

}

//Methods
void    Tile::move(int amount, const Tile& to)
{
	std::cout << "MOVE " << amount << *this << to.x << " " << to.y << ";";
}
void    Tile::move(int amount, int x, int y)
{
	std::cout << "MOVE " << amount << *this << x << " " << y << ";";
}
void    Tile::move(int amount, int id)
{
	std::cout << "MOVE " << amount << *this << tiles->at(id).x << " " << tiles->at(id).y << ";";
}

void	Tile::build()
{
	std::cout << "BUILD" << *this << ";";
}
void	Tile::spawn(int amount)
{
	std::cout << "SPAWN " << amount << *this << ";";
}

void	Tile::read()
{
	std::cin >> scrap_amount >> owner >> units >> recycler >> can_build >> can_spawn >> in_range_of_recycler;
	std::cin.ignore();
	recycled_by_opp = false;
	recycled_by_me = false;
	targeted = false;
}

void Tile::debug() const
{
	::debug("id :", id);
	std::cerr << std::endl;
	::debug("x :", x);
	::debug("y :", y);
	::debug("scrap_amount :", scrap_amount);
	::debug("owner :", owner);
	::debug("units :", units);
	std::cerr << std::endl;
}


Tile*	Tile::getLeft()
{
	return left;
}

Tile*	Tile::getRight()
{
	return right;
}

Tile*	Tile::getTop()
{
	return top;
}

Tile*	Tile::getBottom()
{
	return bottom;
}

std::vector<Tile*>	Tile::getNeighbors()
{
	std::vector<Tile*> neighbors;
	for (auto& neighbor : {left, right, top, bottom})
	{
		if (neighbor)
			neighbors.push_back(neighbor);
	}
	return (neighbors);
}

std::vector<Tile*>	Tile::getNeighbors(function_is_tile f_is_tile)
{
	std::vector<Tile*> neighbors;
	for (auto& neighbor : {left, right, top, bottom})
	{
		if (neighbor && f_is_tile(*neighbor))
			neighbors.push_back(neighbor);
	}
	return (neighbors);
}
int		Tile::countNeighborsUnits(int player)
{
	int count = 0;
	for (auto& neighbor : getNeighbors())
	{
		if (neighbor->owner == player)
			count += neighbor->units;
	}
	return (count);
}

void	Tile::setPotentialUnits()
{
	potential_units = (owner == ME ? units : - units); 
	potential_units += countNeighborsUnits(ME) - countNeighborsUnits(OPP);
}

int		Tile::potentialSupport()
{
	int count = 0;
	for (auto& neighbor : getNeighbors())
	{
		if (neighbor->owner == ME && potential_units >= 0)
			count += neighbor->potential_units;
	}
	return (count);
}


bool	Tile::isRecycledBy(int player)
{
	for (auto& neighbor : getNeighbors())
	{
		if (neighbor->owner == player && is_recycler(*neighbor))
			return (true);
	}
	return (false);
}

bool	Tile::isNextTo(int player)
{
	for (auto& neighbor : getNeighbors())
	{
		if (neighbor->owner == player)
			return (true);
	}
	return (false);
}

bool	Tile::isNextTo(function_is_tile f_is_tile)
{
	for (auto& neighbor : getNeighbors())
	{
		if (f_is_tile(*neighbor))
			return (true);
	}
	return (false);
}

int		Tile::getDistanceTo(int id)
{
	return (distances.at(id));
}
int		Tile::getDistanceTo(Tile& tile)
{
	return (distances.at(tile.id));
}



std::ostream& operator<<(std::ostream& os, const Tile& t)
{
	os << " " << t.x << " " << t.y << " ";
	return os;
}
