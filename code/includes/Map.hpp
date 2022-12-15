#ifndef MAP_HPP
# define MAP_HPP


# include "includes.hpp"
# include "Tile.hpp"

class Map
{
	public:
		Map(int width, int height);

		void	read();

		Tile *	center();

		Tile *	closestEnemy( Tile& to );
		Tile *	closestUnit( Tile& to);

		Tile *	getTile(int x, int y);



	public:
		std::vector<std::vector<Tile> >	_tiles;
		std::vector<Tile*>				_my_tiles;
		std::vector<Tile*>				_opp_tiles;
		int 							_width;
		int 							_height;
		int								_my_matter;
		int								_opp_matter;
};

#endif /* MAP_HPP */
