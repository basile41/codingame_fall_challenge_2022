#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"

int turn;
int my_side;

int main()
{
	// while (1)
	// {
	//     std::string s;
	//     std::getline(std::cin, s);
	//     std::cerr << s << std::endl;
	// }

	turn = 0;
	int width;
	int height;
	std::cin >> width >> height;
	std::cin.ignore();
	// Data d(width, height);

	// game loop
	while (42)
	{
		Data d;
		d.width = width;
		d.height = height;
		d.read();

		if (turn == 0)
			my_side = d.my_tiles[0]->x < width / 2 ? LEFT : RIGHT;

		// std::cerr << "tile 8 " << d.tiles[8].x << " " << d.tiles[8].y << std::endl;
		// std::cerr << "tile 8 " << d.getTile(0, 1)->x << " " << d.getTile(0, 1)->y << std::endl;
		// std::cerr << "tile 8 : id : " << d.getTile(0, 1)->id << std::endl;
		// std::cerr << std:: endl;
		Graph g;
		g.tiles = &d.tiles;

		// set les sommets du graphe
		for (auto &tile : d.tiles)
		{
			if (tile.scrap_amount && !tile.recycler)
			{
				g.addVertex(tile.id);
			}
		}

		// set les arretes du graphe
		for (auto &tile : d.tiles)
		{
			int x = tile.x;
			int y = tile.y;
			if (tile.scrap_amount && !tile.recycler)
			{
				for (auto &neighbor : {d.getUsableTile(x - 1, y),
									   d.getUsableTile(x, y - 1),
									   d.getUsableTile(x + 1, y),
									   d.getUsableTile(x, y + 1)})
				{

					if (neighbor)
					{
						// std::cerr << "neighbor " << neighbor->x << " " << neighbor->y << std::endl;
						// std::cerr << "neighbor id " << neighbor->id << std::endl;

						g.addEdge(tile.id, neighbor->id);
					}
				}
			}
		}

		// build à côté d'une unité ennemie
		for (auto &tile : d.opp_units)
		{

			if (d.my_matter < 10)
				break;
			for (auto &neighbor_id : d.getNeighbors(tile->id, is_my_empty_tile))
			{
				if (d.my_matter < 10)
					break;
				d.tiles.at(neighbor_id).build();
			}
		}

		auto me_it = std::max_element(d.my_units.begin(), d.my_units.end(),
									  [](const Tile *t1, const Tile *t2)
									  { return (t1->x < t2->x) ^ my_side; });
		auto me_unit = *me_it;

		if (d.my_matter >= 10)
			me_unit->spawn(1);

		for (auto &tile : d.my_units)
		{
			int target_id = bfs(g, tile->id, [](Tile &tile)
								{ return (tile.owner == NONE); });
			if (target_id != -1)
				tile->move(tile->units, d.tiles.at(target_id));
		}

		message("coucou");
		std::cout << std::endl;
		turn++;
	}
}
