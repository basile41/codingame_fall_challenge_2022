#ifndef DATA_HPP
# define DATA_HPP

# include "includes.hpp"
# include "Tile.hpp"
# include "Graph.hpp"

bool is_tile(Tile *tile);

class Data
{
	public:
		Data(int width, int height);

		int 	width;
		int 	height;
		int		size;
		int		my_matter;
		int		opp_matter;
		int		my_side;
		int		dir_x;
		int		dir_y;
		int		dist_start_to_center;

        std::vector<Tile>	tiles;
        std::vector<Tile*>	my_tiles;
        std::vector<Tile*>	opp_tiles;
        std::vector<Tile*>	neutral_tiles;
        std::vector<Tile*>	my_units;
        std::vector<Tile*>	opp_units;
        std::vector<Tile*>	my_recyclers;
        std::vector<Tile*>	opp_recyclers;
        std::vector<Tile*>	mid_tiles;

		Graph*	graph;

		void	read();

		void	build(Tile& tile)
		{
			if (my_matter >= 10)
			{
				tile.recycler = true;
				tile.build();
				my_matter -= 10;
			}
			else
				debug("pas assez de matiere pour build");
		}

		void	spawn(Tile& tile, int amount)
		{
			if (my_matter >= 10 * amount)
			{
				tile.spawn(amount);
				my_matter -= 10 * amount;
			}
			else
				debug("pas assez de matiere pour spawn");
		}

		void	setAllDistance();
		void	setMidTiles();

		Tile *	closestMidTileTo(Tile& tile, bool accept_targeted = true)
		{
			Tile*	closest = nullptr;
			int dist_min = 999;
			for (auto& mid_tile : mid_tiles)
			{
				if (is_tile(*mid_tile) && (accept_targeted || mid_tile->owner != TARGETED) && !is_recycler(*mid_tile) )
				{
					int dist = tile.getDistanceTo(*mid_tile);
					if (dist < dist_min)
					{
						dist_min = dist;
						closest = mid_tile;
					}
				}
			}
			return closest;
		}

		Tile *	getTile(int id);
		Tile *	getTile(int x, int y);
		std::vector<Tile*>	getTilesIf(TileCondition is_matching);

		Tile *	getValidTile(int x, int y);
		Tile *	getUsableTile(int x, int y);

		int		getId(int x, int y);


		std::vector<int> getNeighbors(int id, bool (*f)(Tile*));
		std::vector<int> getNeighbors(int id, bool (*f)(Tile*), int dir_x, int dir_y);


		int		getRecycleRent(int id);

		int		getDistance(int id1, int id2);

		Tile *	getClosest(Tile& start, TileCondition is_matching);

		bool	isFirstOfLine(Tile& my_tile, int dir_x);

};

#endif /* DATA_HPP */
