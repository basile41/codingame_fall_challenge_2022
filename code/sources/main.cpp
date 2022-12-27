
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
		Graph	graph;
		init_graph(d, graph);
		d.graph = &graph;
		d.setAllDistance();


		// Calculer nb de robots potentiels pour chaque case au tour suivant (les miens - les ennemis)
		for (auto& tile : d.tiles)
		{
			tile.potential_units = is_me(tile) ? tile.units : - tile.units; 
			tile.potential_units += tile.countNeighborsUnits(ME) - tile.countNeighborsUnits(OPP);
		}

// defense
		// pour chacune de mes tuiles
		for (auto& _my_tile : d.my_tiles) 
		{
			Tile &	my_tile = *_my_tile;

			if (my_tile.isNextTo( make_is_tile(is_opp, is_unit) ))
			{
				if (my_tile.can_build)
				{
					if (0)// si valable de build -> build (à définir)
					{}
					else if (my_tile.potential_units >= 0)
					{

					}
					else
				}
			}

			// if (my_tile.can_build)
			// {
			// 	if (my_tile.isNextTo(OPP))
			// 	{
			// 		int opp_units = my_tile.countNeighborsUnits(OPP);
			// 		int my_units = my_tile.countNeighborsUnits(ME);
			// 		if (opp_units >= 2)
			// 		{
			// 			if (d.my_matter >= 10)
			// 			{
			// 				my_tile.build();
			// 				d.my_matter -= 10;
			// 			}
			// 			else if (my_units >= opp_units)
			// 			{

			// 				for (auto& _neighbors : my_tile.getNeighbors(make_is_tile(is_me, is_unit)))
			// 				{

			// 				}
			// 			}
			// 		}
			// 	}

			}

		}

		message("🦊");
		std::cout << "WAIT;" << std::endl;
		turn++;

	}// while(1)
}
