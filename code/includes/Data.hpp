#ifndef DATA_HPP
# define DATA_HPP


# include "Tile.hpp"
# include "includes.hpp"

bool is_tile(Tile *tile);

class Data
{
	public:
		// Data(int width, int height);

		int width;
		int height;
		int	my_matter;
		int	opp_matter;

        std::vector<Tile>	tiles;
        std::vector<Tile*>	my_tiles;
        std::vector<Tile*>	opp_tiles;
        std::vector<Tile*>	neutral_tiles;
        std::vector<Tile*>	my_units;
        std::vector<Tile*>	opp_units;
        std::vector<Tile*>	my_recyclers;
        std::vector<Tile*>	opp_recyclers;

		int	my_side;

		void	read();

		Tile *	center();

		Tile *	closestEnemy( Tile& to );
		Tile *	closestUnit( Tile& to);


		Tile *	getTile(int x, int y);
		Tile *	getValidTile(int x, int y);
		Tile *	getUsableTile(int x, int y);

		int		getId(int x, int y);


		std::vector<int> getNeighbors(int id, bool (*f)(Tile*));

		int	getRecycleRent(int id);

};

#endif /* DATA_HPP */
