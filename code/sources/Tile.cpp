#include "includes.hpp"
#include "Tile.hpp"


//Methods
void    Tile::move(int amount, const Tile& to)
{
	std::cout << "MOVE " << amount << *this << to.x << " " << to.y << ";";
}
void    Tile::move(int amount, int x, int y)
{
	std::cout << "MOVE " << amount << *this << x << " " << y << ";";
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
	std::cin >> scrap_amount >> owner >> units >> recycler >> can_build >> can_spawn >> in_range_of_recycler; std::cin.ignore();
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


std::ostream& operator<<(std::ostream& os, const Tile& t)
{
	os << " " << t.x << " " << t.y << " ";
	return os;
}
