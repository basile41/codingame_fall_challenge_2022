
#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"

int turn;
int my_side;

int main()
{
	int 	width;
	int 	height;

	turn = 0;
	std::cin >> width >> height;
	std::cin.ignore();
	
	Data	d(width, height);
	
	// game loop
	while (42)
	{
		d.read();


		for (auto& neighbor : d.tiles[0].getNeighbors())
		{
			neighbor->debug();
		}

		// for (auto& tile : d.tiles)
		// {

		// 	if (make_is_tile( is_me, is_not(is_unit) )(tile))
		// 	{
		// 		for (auto& neighbor : tile.getNeighbors())
		// 		{
		// 			neighbor->debug();
		// 		}
		// 	}
		// }

		for (auto& tile : d.my_units)
		{
			tile->move(1, *d.opp_tiles[0]);	 
		}

		message("🦊");
		std::cout << "WAIT;" << std::endl;
		turn++;

	}// while(1)
}
