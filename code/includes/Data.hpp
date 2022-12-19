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


		Tile *	getTile(int x, int y);
		Tile *	getValidTile(int x, int y);
		Tile *	getUsableTile(int x, int y);

		int		getId(int x, int y)
		{
			if (x < 0 || x >= width || y < 0 || y >= height)
				return -1;
			return (y * width + x);
		}


		std::vector<int> getNeighbors(int id, bool (*f)(Tile*))
		{
			std::vector<int> neighbors;
			int x = tiles.at(id).x;
			int y = tiles.at(id).y;
            for (auto &neighbor_id : {getId(x - 1, y),
                                   		getId(x, y - 1),
                                   		getId(x + 1, y),
                                   		getId(x, y + 1)})
			{
				if (neighbor_id != -1 && f(&tiles.at(neighbor_id)))
					neighbors.push_back(neighbor_id);
			}
			return (neighbors);
		}

};

#endif /* DATA_HPP */
