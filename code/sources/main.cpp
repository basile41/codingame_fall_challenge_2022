#include "includes.hpp"
#include "Tile.hpp"
#include "Map.hpp"

int main()
{
    int width;
    int height;
    std::cin >> width >> height; std::cin.ignore();
	Map map(width, height);

	// game loop

	while (1)
	{
		map.read();
		// Tile	*scoot
		// Tile	*target = map.closestEnemy();
		debug(*map.center());
		for (auto tile : map._my_tiles)
		{
			if (tile->_units)
			{
				if (map._my_matter >= 10)
				{
					tile->spawn(1);
					map._my_matter -= 10;
				}
				tile->move(tile->_units, *map.closestEnemy(*tile));
			}
		}

		message("coucou");
		std::cout << std::endl;
	}
}
