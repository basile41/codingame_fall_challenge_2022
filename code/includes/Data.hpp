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

        std::vector<Tile>	tiles;
        std::vector<Tile*>	my_tiles;
        std::vector<Tile*>	opp_tiles;
        std::vector<Tile*>	neutral_tiles;
        std::vector<Tile*>	my_units;
        std::vector<Tile*>	opp_units;
        std::vector<Tile*>	my_recyclers;
        std::vector<Tile*>	opp_recyclers;

		Graph*	graph;

		void	read();

		void	setAllDistance();

		Tile *	getTile(int id);
		Tile *	getTile(int x, int y);

		Tile *	getValidTile(int x, int y);
		Tile *	getUsableTile(int x, int y);

		int		getId(int x, int y);


		std::vector<int> getNeighbors(int id, bool (*f)(Tile*));
		std::vector<int> getNeighbors(int id, bool (*f)(Tile*), int dir_x, int dir_y);


		int		getRecycleRent(int id);

		int		getDistance(int id1, int id2);

		Tile *	getClosest(Tile& start, function_is_tile is_tile);

		bool	isFirstOfLine(Tile& my_tile, int dir_x);

};

#endif /* DATA_HPP */
