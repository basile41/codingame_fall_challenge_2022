#include "Tile.hpp"

Tile::Tile(int x, int y) : _col(x), _row(y)
{
}

Tile::Tile(const Tile& copy)
{
	*this = copy;
}

Tile &	Tile::operator=(const Tile& rhs)
{
	if (this != &rhs)
	{
		_col = rhs._col;
		_row = rhs._row;
		_scrap_amount = rhs._scrap_amount;
		_owner = rhs._owner;
		_units = rhs._units;
		_recycler = rhs._recycler;
		_can_build = rhs._can_build;
		_can_spawn = rhs._can_spawn;
		_in_range_of_recycler = rhs._in_range_of_recycler;
	}
	return *this;
}

//Methods
void    Tile::move(int amount, const Tile& to)
{
	std::cout << "MOVE " << amount << *this << to._col << " " << to._row << ";";
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
void	Tile::message(const std::string& message) const
{
	std::cout << "MESSAGE " << message << ";";
}

void	Tile::read()
{
	std::cin >> _scrap_amount >> _owner >> _units >> _recycler >> _can_build >> _can_spawn >> _in_range_of_recycler; std::cin.ignore();
}

int		Tile::distance(const Tile& to) const
{
	return (abs(_col - to._col) + abs(_row - to._row));
}
int		Tile::distance(int x, int y) const
{
	return (abs(_col - x) + abs(_row - y));
}

int 	Tile::scrapsAround()
{
	int total_amount = _scrap_amount;
	total_amount += left() 	? std::max(_scrap_amount, left()->_scrap_amount) 	: 0;
	total_amount += right() ? std::max(_scrap_amount, right()->_scrap_amount) 	: 0;
	total_amount += up() 	? std::max(_scrap_amount, up()->_scrap_amount) 		: 0;
	total_amount += down() 	? std::max(_scrap_amount, down()->_scrap_amount) 	: 0;
	return total_amount;
}

bool	Tile::isMe() const
{
	return (_owner == 1);
}
bool	Tile::isEnemy() const
{
	return (_owner == 0);
}
bool	Tile::isNeutral() const
{
	return (_owner == -1);
}

Tile *	Tile::left() const
{
	return _left;
}
Tile *	Tile::right() const
{
	return _right;
}
Tile *	Tile::up() const
{
	return _up;
}
Tile *	Tile::down() const
{
	return _down;
}

void	Tile::setLeft(Tile *tile)
{
	_left = tile;
}
void	Tile::setRight(Tile *tile)
{
	_right = tile;
}
void	Tile::setUp(Tile *tile)
{
	_up = tile;
}
void	Tile::setDown(Tile *tile)
{
	_down = tile;
}		


std::ostream& operator<<(std::ostream& os, const Tile& t)
{
	os << " " << t._col << " " << t._row << " ";
	return os;
}
