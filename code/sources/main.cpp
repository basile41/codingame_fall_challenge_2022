#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"

int turn;
int my_side;

void	init_graph(Data& d, Graph& graph)
{
	graph.tiles = &d.tiles;
	// set les sommets du graphe
	for (auto &tile : d.tiles)
	{
		if (tile.scrap_amount && !tile.recycler)
		{
			graph.addVertex(tile.id);
		}
	}

	// set les arretes du graphe
	for (auto &tile : d.tiles)
	{
		if (tile.scrap_amount && !tile.recycler)
		{
			for (auto &neighbor_id : d.getNeighbors(tile.id, is_usable_tile))
			{
				graph.addEdge(tile.id, neighbor_id);
			}
		}
	}
}

// template<typename T>
// std::function<bool(T&, T& )> make_lambda_fcomp(int side)
// {
// 	std::function<bool(T&, T& )> f_comp;
// 	if (side == LEFT)
// 		f_comp = 	[](T& v1, T& v2)->bool 
// 					{ return (v1 < v2);};
// 	else
// 		f_comp = 	[](T& v1, T& v2)->bool 
// 					{ return (v1 > v2);};
// 	return f_comp;
// }

std::function<bool(Tile*&, Tile*& )> make_lambda_fcomp_x(int side, int height)
{
	int y = height / 2;
	std::function<bool(Tile*&, Tile*& )> f_comp;
	if (side == LEFT)
		f_comp = 	[y](Tile*& t1, Tile*& t2)->bool 
		{
			if (t1->x == t2->x)
				return (abs(y - t1->y) < abs(y - t2->y));
			return (t1->x < t2->x);
		};
	else
		f_comp = 	[y](Tile*& t1, Tile*& t2)->bool 
		{
			if (t1->x == t2->x)
				return (abs(y - t1->y) > abs(y - t2->y));
			return (t1->x > t2->x);
		};
	return f_comp;
}


int main()
{
	// while (1)
	// {
	//     std::string s;
	//     std::getline(std::cin, s);
	//     std::cerr << s << std::endl;
	// }

	int 	width;
	int 	height;

	std::function<bool(Tile &tile)> f_to_find;
	// Tile*	me_start;
	// Tile*	opp_start;
	
	turn = 0;
	std::cin >> width >> height;
	std::cin.ignore();
	
	

	// game loop
	while (42)
	{

		// if (turn == 17)
		// {
		// 	std::string str;
		// 	while(std::getline(std::cin, str))
		// 	{
		// 		std::cerr << str << std::endl;
		// 	}
		// }

		Data d;
		d.width = width;
		d.height = height;
		d.read();

		
		std::function<bool(Tile&, Tile& )> f_comp;
		f_comp = 	[](Tile& t1, Tile& t2)->bool 
					{ return (t1.x < t2.x);};

		std::sort(d.my_units.begin(), d.my_units.end(), make_lambda_fcomp_x(my_side, height));

		if (turn == 0)
			my_side = d.my_tiles[0]->x < width / 2 ? LEFT : RIGHT;

		// std::cerr << "tile 8 " << d.tiles[8].x << " " << d.tiles[8].y << std::endl;
		// std::cerr << "tile 8 " << d.getTile(0, 1)->x << " " << d.getTile(0, 1)->y << std::endl;
		// std::cerr << "tile 8 : id : " << d.getTile(0, 1)->id << std::endl;
		// std::cerr << std:: endl;

		// unité la plus proche de mon bord
		// auto my_last_unit_it = std::max_element(d.my_units.begin(), d.my_units.end(),
		// 							  [](const Tile *t1, const Tile *t2)
		// 							  { return (t1->x > t2->x) ^ my_side; });



		// build si >= 2 ennemis a côté sinon spawn 1 si 1 unité 
		for (auto &opp_unit : d.opp_units)
		{
			if (d.my_matter < 10)
				break;
			for (auto &neighbor_id : d.getNeighbors(opp_unit->id, is_my_empty_tile))
			{
				if (d.my_matter < 10)
					break;
				if (opp_unit->units >= 2)
				{
					d.tiles.at(neighbor_id).build();
					d.my_matter -= 10;
				}
				else if (opp_unit->units == 1)
				{
					d.tiles.at(neighbor_id).spawn(1);
					d.my_matter -= 10;
				}
			}
		}

		// build investisssement
		if (d.my_matter >= 10)
		{
			int max_rent = 0;
			int max_rent_id;
			for (auto& my_tile : d.my_tiles)
			{
				if (my_tile->units == 0)
				{
					int rent = d.get_recycle_rent(my_tile->id);
					debug("current: ");
					debug(my_tile->id, rent);

					if (rent > max_rent)
					{
						max_rent = rent;
						max_rent_id = my_tile->id;
					}
				}
			}
			debug("max: ");
			debug(max_rent_id, max_rent);
			if (max_rent >= 10)
			{
				d.tiles.at(max_rent_id).build();
				d.my_matter -= 10;
			}
		}




		Graph graph;
		graph.tiles = &d.tiles;
		init_graph(d, graph);


		// defini mes territoires isolés
		f_to_find = [](Tile &tile)->bool 
					{ return (tile.owner == OPP); };
		for (auto &my_tile : d.my_tiles)
		{
			if (!my_tile->recycler && my_tile->units == 0 && !my_tile->isolated)
			{
				if (bfs(graph, my_tile->id, f_to_find) == -1)
				{
					for (auto& visited : graph.visited)
					{
						d.tiles[visited].isolated = true;
					}
				}
			}
		}

		


		for (auto &opp_unit : d.opp_tiles)
		{
			if (d.my_matter < 10)
				break;
			
			if (opp_unit->units == 0 && !opp_unit->recycler)
			{
				for (auto &neighbor_id : d.getNeighbors(opp_unit->id, is_my_empty_tile))
				{
					if (d.my_matter < 10)
						break;
					d.tiles.at(neighbor_id).spawn(1);
					d.my_matter -= 10;
					break ;
				}

			}
		}


		std::vector<Tile *>::iterator my_first_unit_it;

		if (!d.my_units.empty())
		{
			// my_first_unit_it = std::max_element(d.my_units.begin(), d.my_units.end(),
			// 							[](const Tile *t1, const Tile *t2)
			// 							{ return (t1->x < t2->x) ^ my_side; });
			std::random_shuffle(d.my_units.begin(), d.my_units.end());

			auto unit_to_spawn = d.my_units.front();

			if (d.my_matter >= 10)
			{
				f_to_find = [](Tile &tile)->bool 
							{ return (tile.owner == OPP); };
				if (bfs(graph, unit_to_spawn->id, f_to_find) != -1)
				{
					unit_to_spawn->spawn(1);
					d.my_matter -= 10;
				}
			}

		}


		//spawn sur terrain isolé
		if (d.my_matter >= 10)
		{
			f_to_find = [](Tile &tile)->bool 
						{ return (tile.units); };
			for (auto &my_tile : d.my_tiles)
			{
				if (my_tile->isolated && !my_tile->units && bfs(graph, my_tile->id, f_to_find) == -1)
				{
					my_tile->spawn(1);
					d.my_matter -= 10;
				}
			}
		}

		// if (d.my_matter >= 20)
		// {
		// 	for (auto &tile : d.my_tiles)
		// 	{
		// 		if (!tile->isolated)
		// 		{
		// 			tile->spawn(1);
		// 			d.my_matter -= 10;
		// 			break ;
		// 		}
		// 	}

		// }



		//pour chaque units : move vers la case neutre la plus proche
		for (auto &current : d.my_units)
		{
			f_to_find = [](Tile &tile)->bool 
				{ return (tile.owner == OPP); };

			for (auto& neighbor : d.getNeighbors(current->id, is_empty_opp))
			{
				current->move(2, d.tiles.at(neighbor));
			}

			int target_id;


			target_id = bfs(graph, current->id, f_to_find);
			int x = current->x;

			if (target_id == -1)
				f_to_find = [x](Tile &tile)->bool 
					{ return (tile.owner == NONE); };
			else if (my_side == LEFT)
				f_to_find = [x](Tile &tile)->bool 
					{ return (tile.owner == NONE) && tile.x >= x; };
			else
				f_to_find = [x](Tile &tile)->bool 
					{ return (tile.owner == NONE) && tile.x <= x; };

			target_id = bfs(graph, current->id, f_to_find);



			if (target_id != -1) //move vers la case neutre la plus proche
				current->move((current->units + 1) / 2, d.tiles.at(target_id));
			else //move vers la case ennemie la plus proche
			{

				f_to_find = [](Tile &tile)->bool 
					{ return (tile.owner == OPP); };
				target_id = bfs(graph, current->id, f_to_find);
				if (target_id != -1)
					current->move(current->units, d.tiles.at(target_id));
			}
		}

		std::cerr << "matierau restant : " << d.my_matter << std::endl;
		message("La meute");
		std::cout << "WAIT;" << std::endl;
		turn++;
	}
}
