#ifndef MAP_HPP
# define MAP_HPP


# include "includes.hpp"
# include "Tile.hpp"

class Map
{
	public:
		Map(int width, int height);

		void	read();

		Tile &	tileAt(int x, int y);

		Tile &	center();

		const Tile &closestEnemy( const Tile& to ) const;



	private:
		int		_id(const Tile& tile) const;

	public:
		std::vector<std::vector<Tile> >	_tiles;
		int 					_width;
		int 					_height;
		int						_my_matter;
		int						_opp_matter;
};

#endif /* MAP_HPP */
