#ifndef TILE_HPP
# define TILE_HPP

# include "includes.hpp"

class Tile
{
	public:

		Tile(int x, int y);

		Tile(const Tile& copy);

		Tile &	operator=(const Tile& rhs);

		//Methods
		void    move(int amount, const Tile& to);
		void    move(int amount, int x, int y);

		void	build();
		void	spawn(int amount);
		void	message(const std::string& message) const;

		void	read();

		int		distance(const Tile& to) const;
		int		distance(int x, int y) const;

        int scrapsAround();

		bool	isMe() const;
		bool	isEnemy() const;
		bool	isNeutral() const;

		Tile *	left() const;
		Tile *	right() const;
		Tile *	up() const;
		Tile *	down() const;

		void	setLeft(Tile *tile);
		void	setRight(Tile *tile);
		void	setUp(Tile *tile);
		void	setDown(Tile *tile);


		int _col;
		int _row;
		int _scrap_amount;
		int _owner; // 1 = me, 0 = foe, -1 = neutral
		int _units;
		int _recycler;
		int _can_build;
		int _can_spawn;
		int _in_range_of_recycler;

		Tile	*_left;
		Tile	*_right;
		Tile	*_up;
		Tile	*_down;
};

std::ostream& operator<<(std::ostream& os, const Tile& t);

#endif /* TILE_HPP */
