#ifndef DATA_HPP
# define DATA_HPP


# include "includes.hpp"
// # include "Tile.hpp"

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


		//Methods
		void    move(int amount, const Tile& to);
		void    move(int amount, int x, int y);
		void	build();
		void	spawn(int amount);

		void	read();
};

std::ostream& operator<<(std::ostream& os, const Tile& t);

class Data
{
	public:
		// Data(int width, int height);

		int 							width;
		int 							height;
		int								my_matter;
		int								opp_matter;

        std::vector<Tile>	tiles;
        std::vector<Tile*>	my_tiles;
        std::vector<Tile*>	opp_tiles;
        std::vector<Tile*>	neutral_tiles;
        std::vector<Tile*>	my_units;
        std::vector<Tile*>	opp_units;
        std::vector<Tile*>	my_recyclers;
        std::vector<Tile*>	opp_recyclers;

		void	read();

		Tile *	center();

		Tile *	closestEnemy( Tile& to );
		Tile *	closestUnit( Tile& to);

		Tile *	getTile(int x, int y);



};

#endif /* DATA_HPP */
