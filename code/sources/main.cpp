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

		// int my_matter;
		// int opp_matter;
		// std::cin >> my_matter >> opp_matter; std::cin.ignore();
		map.read();
		// map.tileAt(2, 3).spawn(5);
		// for (int i = 0; i < height; i++) {
		// 	for (int j = 0; j < width; j++) {
				// Tile s;
				// int scrap_amount;
				// int owner; // 1 = me, 0 = foe, -1 = neutral
				// int units;
				// int recycler;
				// int can_build;
				// int can_spawn;
				// int in_range_of_recycler;
				// std::cin >> scrap_amount >> owner >> units >> recycler >> can_build >> can_spawn >> in_range_of_recycler; std::cin.ignore();
		// 	}
		// }

		for (auto &row : map._tiles)
		{
			for (auto &tile : row)
			{
				if (tile.isMe())
				{
					if (map._my_matter >= 10)
					{
						tile.spawn(1);
						map._my_matter -= 10;
					}
					if (tile._units)
					{
						tile.move(tile._units, map.closestEnemy(tile));
					}
				}
			}
		}

		std::cout << std::endl;
	}
}
