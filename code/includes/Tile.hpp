#ifndef TILE_HPP
# define TILE_HPP

# include "includes.hpp"

bool	is_mid_tile(Tile& tile);

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
		bool	is_mid_tile;
		int		def_units;
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
		int		dist_to_start;

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
		std::vector<Tile*>	getNeighbors(TileCondition is_matching);
		std::vector<Tile*>	getNeighbors(TileCondition is_matching, TileCondition priority);

		int		getRecyclingValue(int player);
		int		countNeighborsUnits(int player);
		int		potentialUnits();
		int		potentialSupport();
		bool	isRecycledBy(int player);
		bool	isNextTo(int player);
		bool	isNextTo(TileCondition is_matching);

		bool	isNearestThan(Tile& tile, TileCondition is_matching)
		{
			for (auto& current : *tiles)
			{
				if (&current != &tile && is_matching(current))
				{
					if (this->getDistanceTo(current) < tile.getDistanceTo(current))
						return true;
				}
			}
			return false;
		}

		bool	isNearestToMid(Tile& tile)
		{
			int this_dist = 900;
			int tile_dist = 900;
			for (auto& current : *tiles)
			{
				if (&current != &tile && current.is_mid_tile)
				{
					// ::debug("mid tile :", current);
					this_dist = std::min(this_dist, this->getDistanceTo(current) - current.dist_to_start + 1);
					tile_dist = std::min(tile_dist, tile.getDistanceTo(current) - current.dist_to_start);
					// ::debug("this_dist :", this_dist);
					// ::debug("tile_dist :", tile_dist);
				}
			}
			if (this_dist < 900)
				return this_dist <= tile_dist;
			return
				false;
		}

		int		getDistanceTo(int id);
		int		getDistanceTo(Tile& tile);
};


std::ostream& operator<<(std::ostream& os, const Tile& t);


#endif /* TILE_HPP */
