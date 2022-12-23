#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"

int turn;
int my_side;

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
	int 	width;
	int 	height;

	std::function<bool(Tile &tile)> f_to_find;
	int	my_start_id;
	int	opp_start_id;
	int can_do_backward = 0;
	
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
		{
			Graph g;
			init_graph(d, g);
			my_side = d.my_tiles[0]->x < width / 2 ? LEFT : RIGHT;
			for (auto& my_tile : d.my_tiles)
			{

				f_to_find = [](Tile &tile)->bool 
							{ return (tile.owner == OPP && tile.units == 0); };

				if (my_tile->units == 0)
					my_start_id = my_tile->id;
			}
			opp_start_id = bfs(g, my_start_id, f_to_find);
		}
		if (d.tiles[opp_start_id].scrap_amount == 0 || d.tiles[opp_start_id].recycler)
		{
			Graph g;
			init_graph(d, g);
			for (auto& my_tile : d.my_tiles)
			{

				f_to_find = [](Tile &tile)->bool 
							{ return (tile.owner == OPP && tile.units == 0); };

				if (my_tile->units == 0)
					my_start_id = my_tile->id;
			}
			opp_start_id = bfs(g, my_start_id, f_to_find);
		}
		

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
				can_do_backward = 1;
				if (d.my_matter < 10)
					break;
				if (opp_unit->units >= 2)
				{
					d.tiles.at(neighbor_id).build();
					d.my_matter -= 10;
				}
				else if (opp_unit->units == 1)
				{
					if (!d.tiles.at(neighbor_id).isolated)
					{
						d.tiles.at(neighbor_id).spawn(1);
						d.my_matter -= 10;
						
					}
				}
			}
		}



		Graph graph;
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

		

		// spawn a côté d une case vide ennemie
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


		// build investisssement
		if (d.my_matter >= 10)
		{
			int max_rent = 0;
			int max_rent_id;
			for (auto& my_tile : d.my_tiles)
			{
				if (my_tile->units == 0)
				{
					int rent = d.getRecycleRent(my_tile->id);

					if (rent > max_rent)
					{
						max_rent = rent;
						max_rent_id = my_tile->id;
					}
				}
			}
			if (max_rent >= 10)
			{
				d.tiles.at(max_rent_id).build();
				d.my_matter -= 10;
			}
		}



		// actions pour ma case la plus proche de la base ennemi 
		if (turn % 3 && d.my_matter >= 10)
		{
			f_to_find = [](Tile &tile)->bool 
						{ return (tile.owner == ME); };
			int closest_of_ennemi = bfs(graph, opp_start_id, f_to_find);


			f_to_find = [](Tile &tile)->bool 
						{ return (tile.owner == OPP); };
			if (closest_of_ennemi != -1 && !d.tiles[closest_of_ennemi].isolated)
			{
				d.tiles[closest_of_ennemi].spawn(1);
				d.my_matter -= 10;
			}
		}

		// actions pour mon robot le plus proche de la base ennemi 
		// if (d.my_matter >= 10)
		{
			f_to_find = [](Tile &tile)->bool 
						{ return (tile.owner == ME && tile.units); };
			int closest_of_ennemi = bfs(graph, opp_start_id, f_to_find);
			if (closest_of_ennemi != -1)
			{
				d.tiles[closest_of_ennemi].move(1, d.tiles[opp_start_id]);
			}
		}
			


		// spawn haut/bas
		std::function<bool (Tile *&, Tile *&)> fcomp;
		if (turn % 2)
			fcomp = [](Tile*& t1, Tile*& t2)->bool 
			{
				if (t1->y == t2->y)
					return (t1->x > t2->x); //ici
				return (t1->y < t2->y);
			};
		else
			fcomp = [](Tile*& t1, Tile*& t2)->bool 
			{
				if (t1->y == t2->y)
					return (t1->x > t2->x); //ici
				return (t1->y > t2->y);
			};

		if (my_side == LEFT)
			std::sort(d.my_tiles.begin(), d.my_tiles.end(), fcomp);
		else
			std::sort(d.my_tiles.rbegin(), d.my_tiles.rend(), fcomp);
		
		if (!d.my_tiles[0]->isolated)
		{
			d.my_tiles[0]->spawn(1);
			d.my_matter -= 10;
		}
		



		// spawn random
		if (!d.my_units.empty())
		{
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



		//pour chaque units : move vers la case neutre la plus proche
		for (auto &current : d.my_units)
		{
			// si case vide ennemie voisine, essaye de move vers elle
			f_to_find = [](Tile &tile)->bool 
				{ return (tile.owner == OPP); };
			for (auto& neighbor : d.getNeighbors(current->id, is_empty_opp))
			{
				current->move(2, d.tiles.at(neighbor));
			}

			int target_id;
			target_id = bfs(graph, current->id, f_to_find);
			int x = current->x;

			if (target_id == -1 || can_do_backward)
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
			{
				current->move((current->units + 1) / 2, d.tiles.at(target_id));
				current->targeted = true;
			}
			else //move vers la case ennemie la plus proche
			{

				f_to_find = [](Tile &tile)->bool 
					{ return (tile.owner == OPP); };
				target_id = bfs(graph, current->id, f_to_find);
				if (target_id != -1)
					current->move(d.tiles[target_id].units + 1, d.tiles.at(target_id));
			}
		}

		//test
		for (auto &current : d.my_units)
		{
			f_to_find = [](Tile &tile)->bool 
				{ return (tile.owner == OPP); };
			int target_id = bfs(graph, current->id, f_to_find);
			if (target_id != -1)
				current->move(current->units, d.tiles.at(target_id));

		}



		std::cerr << "matierau restant : " << d.my_matter << std::endl;
		nb_bfs();
		message("La meute");
		std::cout << "WAIT;" << std::endl;
		turn++;
	}
}
