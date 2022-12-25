#ifndef TILE_HPP
# define TILE_HPP

# include "includes.hpp"


class Tile
{
	public:

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
		bool	isolated;
		bool	targeted;
		std::vector<int> distances;

		//Methods
		void    move(int amount, const Tile& to);
		void    move(int amount, int x, int y);
		void	build();
		void	spawn(int amount);


		void	read();
		void	debug() const;

		int		getDistanceTo(int id)
		{
			return (distances[id]);
		}
};


std::ostream& operator<<(std::ostream& os, const Tile& t);


#endif /* TILE_HPP */
