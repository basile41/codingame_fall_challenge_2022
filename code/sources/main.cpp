
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
	int	my_start_id = -1;
	int	opp_start_id = -1;
	int dir_x = 1;
	int dir_y = 1;
	int can_do_backward = 0;
	bool	spread_phase = true;
	
	turn = 0;
	std::cin >> width >> height;
	std::cin.ignore();
	
	
	// game loop
	while (42)
	{

		// if (turn == 0)
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
		d.dir_x = dir_x;
		d.dir_y = dir_y;

		
		std::function<bool(Tile&, Tile& )> f_comp;
		f_comp = 	[](Tile& t1, Tile& t2)->bool 
					{ return (t1.x < t2.x);};

		std::sort(d.my_units.begin(), d.my_units.end(), make_lambda_fcomp_x(my_side, height));

		if (turn == 0) // 1er tour
		{
			Graph g;
			init_graph(d, g);
			my_side = d.my_tiles[0]->x < width / 2 ? LEFT : RIGHT;
			dir_x = my_side == LEFT ? 1 : -1;
			for (auto& my_tile : d.my_tiles)
			{
				if (my_tile->units == 0)
				{
					my_start_id = my_tile->id;
				}
			}

			f_to_find = [](Tile &tile)->bool 
						{ return (tile.owner == OPP && tile.units == 0); };
			opp_start_id = bfs(g, my_start_id, f_to_find);
std::cerr << opp_start_id << std::endl;
std::cerr << d.tiles[my_start_id].y << " " << d.tiles[opp_start_id].y << std::endl;
			if (d.tiles[my_start_id].y < d.tiles[opp_start_id].y)
				dir_y = 1;
			else if (d.tiles[my_start_id].y > d.tiles[opp_start_id].y)
				dir_y = -1;
			else
				dir_y = d.tiles[my_start_id].y < height / 2 ? 1 : -1;
		}
		if (d.tiles[opp_start_id].scrap_amount == 0 || d.tiles[opp_start_id].recycler)
		{
			Graph g;
			init_graph(d, g);
			for (auto& my_tile : d.my_tiles)
			{


				if (my_tile->units == 0)
					my_start_id = my_tile->id;
			}
			f_to_find = [](Tile &tile)->bool 
						{ return (tile.owner == OPP && tile.units == 0); };
			opp_start_id = bfs(g, my_start_id, f_to_find);
		}
		d.my_side = my_side;
		d.dir_x = dir_x;
		d.dir_y = dir_y;
		

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
				spread_phase = false;
				if (d.my_matter < 10)
					break;
				if (opp_unit->units >= 1)
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



    // temps courant, avant l'execution
    std::chrono::high_resolution_clock::time_point a= std::chrono::high_resolution_clock::now();
    
    // faire quelquechose
	
		for (auto& tile : d.tiles)
		{
			if (tile.scrap_amount && !tile.recycler)
			{
				for (auto& vertex : graph.vertices)
					vertex.distance = 999;
				bfs(graph, tile.id);
				// std::cerr << tile.id << std::endl;
				for (auto& vertex : graph.vertices)
				{
					tile.distances[vertex.id] = vertex.distance;
				}
			}
		}
    
    // temps courant, apres l'execution
    std::chrono::high_resolution_clock::time_point b= std::chrono::high_resolution_clock::now();
    
    // mesurer la difference, et l'exprimer en microsecondes 
    float time= (float)std::chrono::duration_cast<std::chrono::microseconds>(b - a).count() / 1000;

	std::cerr << "temps d exec : " << time << " ms" << std::endl;





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
				if (my_tile->units == 0 && !my_tile->isolated)
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



		// spawn sur ma case la plus proche de la base ennemi 
		if (d.my_matter >= 10)
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

		// // actions pour mon robot le plus proche de la base ennemi 
		// // if (d.my_matter >= 10)
		// {
		// 	f_to_find = [](Tile &tile)->bool 
		// 				{ return (tile.owner == ME && tile.units); };
		// 	int closest_of_ennemi = bfs(graph, opp_start_id, f_to_find);
		// 	if (closest_of_ennemi != -1)
		// 	{
		// 		d.tiles[closest_of_ennemi].move(1, d.tiles[opp_start_id]);
		// 	}
		// }


		// spawn 1ere ligne
		if (spread_phase)
		{
			//ici
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

		//test
		for (auto& my_unit : d.my_units)
		{
			std::cerr << *my_unit << " is first of line : " << d.isFirstOfLine(*my_unit, dir_x) << std::endl;	
		}

		//refonte movement
// if (spread_phase)
{
		for (auto& my_unit : d.my_units)
		{
			int x = my_unit->x;
			int y = my_unit->y;
			f_to_find = [](Tile &tile)->bool 
						{ return (tile.owner == OPP); };
			int closest_opp = bfs(graph, my_unit->id, f_to_find);
			if (my_unit->isolated || (closest_opp != -1 && my_unit->distances[closest_opp] <= 3))
				continue ;
			if (d.isFirstOfLine(*my_unit, dir_x))
			{
				// Tile* forward_tile = d.getUsableTile(x + dir_x);

				f_to_find = [](Tile &tile)->bool 
							{ return (tile.owner == OPP); };
				int closest_opp = bfs(graph, my_unit->id, f_to_find);
				if (closest_opp != -1)
				{
					f_to_find = [](Tile &tile)->bool 
								{ return (tile.owner == NONE); };
					int target_id = bfs(graph, my_unit->id, closest_opp, f_to_find);
					if (target_id != -1)
					{
						my_unit->move(1, d.tiles[target_id]);
						my_unit->units--;
					}
				}


			}
			
			if (my_unit->units)
			{
				std::cerr << "dir_y" << dir_y << std::endl;
				for (auto& neighbor : {	d.getUsableTile(x, y + dir_y),
										d.getUsableTile(x, y - dir_y)})
				{
					if (neighbor && neighbor->owner == NONE)
					{
						my_unit->move(1, *neighbor);
						my_unit->units--;
						break ;
					}
				}

				if (my_unit->units)
				{
					for (auto& neighbor : {	d.getUsableTile(x, y + 2 * dir_y),
											d.getUsableTile(x, y - 2 * dir_y)})
					{
						if (neighbor && neighbor->owner == NONE)
						{
							my_unit->move(1, *neighbor);
							my_unit->units--;
							break ;
						}
					}
				}
			// 	if (my_unit->units)
			// 	{
			// std::cerr << "coucou" << std::endl;
			// 		f_to_find = [](Tile &tile)->bool 
			// 					{ return (tile.owner == OPP); };
			// 		int closest_opp = bfs(graph, my_unit->id, f_to_find);

			// 		f_to_find = [](Tile &tile)->bool 
			// 					{ return (tile.owner == NONE); };
			// 		int target_id = bfs(graph, my_unit->id, closest_opp, f_to_find);
			// std::cerr << "coucou2" << std::endl;
			// 		if (target_id != -1)
			// 		{
			// 			my_unit->move(my_unit->units, d.tiles[target_id]);
			// 			my_unit->units = 0;
			// 		}
			// 	}
				
			}
		}


}


// else
{
// 		//pour chaque units : move vers la case neutre la plus proche
// 		for (auto &current : d.my_units)
// 		{
// 			// si case vide ennemie voisine, essaye de move vers elle
// 			f_to_find = [](Tile &tile)->bool 
// 				{ return (tile.owner == OPP); };
// 			for (auto& neighbor : d.getNeighbors(current->id, is_empty_opp))
// 			{
// 				current->move(2, d.tiles.at(neighbor));
// 			}

// 			int target_id;
// 			target_id = bfs(graph, current->id, f_to_find);
// 			int x = current->x;

// 			if (target_id == -1 || can_do_backward)
// 				f_to_find = [x](Tile &tile)->bool 
// 					{ return (tile.owner == NONE); };
// 			else if (my_side == LEFT)
// 				f_to_find = [x](Tile &tile)->bool 
// 					{ return (tile.owner == NONE) && tile.x >= x; };
// 			else
// 				f_to_find = [x](Tile &tile)->bool 
// 					{ return (tile.owner == NONE) && tile.x <= x; };

// 			target_id = bfs(graph, current->id, f_to_find);



// 			if (target_id != -1) //move vers la case neutre la plus proche
// 			{
// 				current->move((current->units + 1) / 2, d.tiles.at(target_id));
// 				current->targeted = true;
// 			}
// 			else //move vers la case ennemie la plus proche
// 			{

// 				f_to_find = [](Tile &tile)->bool 
// 					{ return (tile.owner == OPP); };
// 				target_id = bfs(graph, current->id, f_to_find);
// 				if (target_id != -1)
// 					current->move(d.tiles[target_id].units + 1, d.tiles.at(target_id));
// 			}
// 		}

		//test
		for (auto &current : d.my_units)
		{
			if (current->isolated)
			{

				f_to_find = [](Tile &tile)->bool 
					{ return (tile.owner == NONE); };
			}
			else
			{
				f_to_find = [](Tile &tile)->bool 
					{ return (tile.owner == OPP); };
			}
			int target_id = bfs(graph, current->id, f_to_find);
			std::cerr << "target_id : " << target_id << std::endl;
			if (target_id != -1 && current->units)
			{
				current->move(current->units, d.tiles.at(target_id));
				current->units = 0;
			}

		}
}


		std::cerr << "materiaux restant : " << d.my_matter << std::endl;
		nb_bfs();
		message("🦊");
		std::cout << "WAIT;" << std::endl;
		turn++;

	}// while(1)
}
