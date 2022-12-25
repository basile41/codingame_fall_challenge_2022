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
		int	dir_x;
		int	dir_y;

		void	read();

		Tile *	center();

		Tile *	closestEnemy( Tile& to );
		Tile *	closestUnit( Tile& to);


		Tile *	getTile(int x, int y);
		Tile *	getValidTile(int x, int y);
		Tile *	getUsableTile(int x, int y);

		int		getId(int x, int y);


		std::vector<int> getNeighbors(int id, bool (*f)(Tile*));
		std::vector<int> getNeighbors(int id, bool (*f)(Tile*), int dir_x, int dir_y);


		int		getRecycleRent(int id);

		int		getDistance(int id1, int id2)
		{
			return (tiles[id1].distances[id2]);
		}

		bool	isFirstOfLine(Tile& my_tile, int dir_x)
		{
			int y = my_tile.y;
			int border = -1;

			if (dir_x == 1)
				border = width;
			for (int x = my_tile.x + dir_x ; x != border; x += dir_x)
			{
				if (getTile(x, y)->owner == 1 && getTile(x, y)->units)
					return (false);
			}
			return (true);
		}

};

#endif /* DATA_HPP */
