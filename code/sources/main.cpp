
#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"

int turn;
int my_side;

void	move_support_from_neighbor(Tile& tile, int requires)
{
	auto neighbors = tile.getNeighbors(is_me);

	for (auto& neighbor : tile.getNeighbors(is_me)) // move les renforts voisns
	{
		int neighbor_support = neighbor->potentialUnits() - tile.units;
		neighbor_support = std::min({neighbor_support, neighbor->units, requires});
		neighbor->move(neighbor_support, tile);
		neighbor->units -= neighbor_support;
		requires -= neighbor_support;
		if (requires <= 0)
			break ;
	}
}

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
		// for (auto& tile : d.tiles)
		// {
		// 	tile.potential_units = is_me(tile) ? tile.units : - tile.units; 
		// 	tile.potential_units += tile.countNeighborsUnits(ME) - tile.countNeighborsUnits(OPP);
		// }

// defense
		// pour chacune de mes tuiles
		for (auto& _my_tile : d.my_tiles) 
		{
			Tile &	my_tile = *_my_tile;

			if (my_tile.isNextTo( make_is_tile(is_opp, is_unit) ))
			{

				int opp_neighbor_units = my_tile.countNeighborsUnits(OPP);
				if (my_tile.can_build)
				{
					if (0)// si valable de build -> build (à définir)
					{}
					else if (my_tile.potentialSupport() >= opp_neighbor_units)
					{
						move_support_from_neighbor(my_tile, opp_neighbor_units);
					}
					else if (d.my_matter >= 10)
					{
						d.build(my_tile);
					}
				}
				else if (my_tile.potentialSupport() + my_tile.units >= opp_neighbor_units)
				{
					move_support_from_neighbor(my_tile, opp_neighbor_units - my_tile.units);
				}
				else // spawn ?
				{

				}
			}
		}

		message("🦊");
		std::cout << "WAIT;" << std::endl;
		turn++;

	}// while(1)
}
