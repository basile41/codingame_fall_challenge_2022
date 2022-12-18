#ifndef DATA_HPP
# define DATA_HPP


# include "Tile.hpp"
# include "includes.hpp"

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

		void	setNeighbors();

		Tile *	getTile(int x, int y);
		Tile *	getValidTile(int x, int y);
		Tile *	getUsableTile(int x, int y);

};

#endif /* DATA_HPP */
