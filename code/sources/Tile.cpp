#include "includes.hpp"
#include "Tile.hpp"


Tile::Tile(size_t nb_of_tiles)
: left(nullptr), right(nullptr), top(nullptr), bottom(nullptr), distances(nb_of_tiles, 999)
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

int		Tile::getDistanceTo(int id)
{
	return (distances[id]);
}



std::ostream& operator<<(std::ostream& os, const Tile& t)
{
	os << " " << t.x << " " << t.y << " ";
	return os;
}
