#ifndef TILE_HPP
# define TILE_HPP

# include "includes.hpp"


class Tile
{
	public:
		
		Tile(size_t nb_of_tiles);

		int		id;
		int		x;
		int		y;
		int		scrap_amount;
		int		owner; // 1 = me, 0 = foe, -1 = neutral
		int		units;
		bool	recycler;
		bool	can_build;
		bool	can_spawn;
		bool	in_range_of_recycler;

		// int		potential_units;
		bool	recycled_by_opp;
		bool	recycled_by_me;
		bool	isolated;
		bool	targeted;
		Tile*	left;
		Tile*	right;
		Tile*	top;
		Tile*	bottom;
		std::vector<Tile> *tiles;
		std::vector<int> distances;

		//Methods
		void    move(int amount, const Tile& to);
		void    move(int amount, int x, int y);
		void    move(int amount, int id);
		void	build();
		void	spawn(int amount);


		void	read();
		void	debug() const;

		//accessors

		Tile*	getLeft();
		Tile*	getRight();
		Tile*	getTop();
		Tile*	getBottom();

		std::vector<Tile*>	getNeighbors();
		std::vector<Tile*>	getNeighbors(function_is_tile f_is_tile);

		int		countNeighborsUnits(int player);
		int		potentialUnits();
		int		potentialSupport();
		bool	isRecycledBy(int player);
		bool	isNextTo(int player);
		bool	isNextTo(function_is_tile f_is_tile);

		int		getDistanceTo(int id);
		int		getDistanceTo(Tile& tile);
};


std::ostream& operator<<(std::ostream& os, const Tile& t);


#endif /* TILE_HPP */
