
#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"

int turn;
int my_side;

void	move_support_from_neighbor(Tile& tile, int required)
{
	auto neighbors = tile.getNeighbors((TileCondition)is_me);

	for (auto& neighbor : tile.getNeighbors((TileCondition)is_me)) // move les renforts voisns
	{
		int neighbor_support = neighbor->potentialUnits() - tile.units;
		neighbor_support = std::min({neighbor_support, neighbor->units, required});
		if (neighbor_support >= 1)
		{
			neighbor->move(neighbor_support, tile);
			neighbor->units -= neighbor_support;
			required -= neighbor_support;
			if (required <= 0)
				break ;
		}
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

// temps courant, avant l'execution
std::chrono::high_resolution_clock::time_point a= std::chrono::high_resolution_clock::now();

		Graph	graph;
		init_graph(d, graph);
		d.graph = &graph;

		d.setAllDistance();
		if (turn == 0)
		{
			d.setMidTiles();
		}

		// test voisins prioritaires
		


// defense
		// pour chacune de mes tuiles
		for (auto& _my_tile : d.my_tiles) 
		{
			Tile &	my_tile = *_my_tile;

			if (is_recycler(my_tile))
				continue ;
			if (my_tile.isNextTo( make_is_matching(is_opp, is_unit) ))
			{
				// debug("my_tile", my_tile);
				int opp_neighbor_units = my_tile.countNeighborsUnits(OPP);
				int required = opp_neighbor_units - my_tile.units;
				if (my_tile.can_build)
				{
					// debug("can_build");
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
					// debug("move support");
					move_support_from_neighbor(my_tile, required);

					my_tile.units = std::max(_my_tile->units - opp_neighbor_units, 0);
				}
				else if (d.my_matter / 10 >= required)
				{
					// debug("move support et spawn");
					move_support_from_neighbor(my_tile, required);
					d.spawn(my_tile, required);
					my_tile.units = std::max(_my_tile->units - opp_neighbor_units, 0);
				}
			}
		}

// defini mes territoires isolés
		for (auto &my_tile : d.my_tiles)
		{
			if (!my_tile->recycler && my_tile->units == 0 && !my_tile->isolated)
			{
				if (bfs(graph, my_tile->id, is_opp) == -1)
				{
					for (auto& visited : graph.visited)
					{
						d.tiles[visited].isolated = true;
					}
				}
			}
		}


// build investissement (à ameliorer)
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

// defini mes territoires isolés
		for (auto &my_tile : d.my_tiles)
		{
			if (!my_tile->recycler && my_tile->units == 0 && !my_tile->isolated)
			{
				if (bfs(graph, my_tile->id, is_opp) == -1)
				{
					for (auto& visited : graph.visited)
					{
						d.tiles[visited].isolated = true;
					}
				}
			}
		}

// tri avant spread
		// trier mes units (plus proches à sa mid tile la plus proche)
		TileCompare _comp_dist_to_mid;
		_comp_dist_to_mid = [&](Tile& t1, Tile& t2)-> bool
		{
			int t1_dist_min = 999;
			int t2_dist_min = 999;
			for (auto& mid_tile : d.mid_tiles)
			{
				t1_dist_min = std::min(t1_dist_min, t1.getDistanceTo(*mid_tile));
				t2_dist_min = std::min(t2_dist_min, t2.getDistanceTo(*mid_tile));
			}
			return (t1_dist_min < t2_dist_min);
		};

		std::function<bool(Tile* t1, Tile* t2)> comp_dist_to_mid;
		comp_dist_to_mid = [&](Tile* t1, Tile* t2)-> bool
		{
			return (_comp_dist_to_mid(*t1, *t2));
		};


		int first_unit = -1;
		int head_unit = -1;
		int last_unit = -1;

		std::vector<Tile*> parents;
		for (auto& my_unit : d.my_units)
		{
			Tile* closest_mid_to_my_unit = d.closestMidTileTo(*my_unit);
			if (closest_mid_to_my_unit == nullptr)
				break;
			if (my_unit->getDistanceTo(*closest_mid_to_my_unit) - closest_mid_to_my_unit->dist_to_start + 1 == -turn)
			{
				parents.push_back(my_unit);
			}
		}
		debug("nombre de parents", parents.size());
		if (parents.size() == 3)
		{
			first_unit = parents[0]->id;
			head_unit = parents[1]->id;
			last_unit = parents[2]->id;

			debug("first_unit" , d.tiles.at(first_unit));
			debug("head_unit" , d.tiles.at(head_unit));
			debug("last_unit" , d.tiles.at(last_unit));
			
		}
		else if (parents.size() == 2)
		{
			first_unit = parents[0]->id;
			last_unit = parents[1]->id;

			debug("first_unit" , d.tiles.at(first_unit));
			debug("last_unit" , d.tiles.at(last_unit));
		}
		else if (parents.size() == 2)
		{
			head_unit = parents[1]->id;
			debug("head_unit" , d.tiles.at(head_unit));
		}


		std::sort(d.my_units.begin(), d.my_units.end(), comp_dist_to_mid);
		std::sort(d.my_tiles.begin(), d.my_tiles.end(), comp_dist_to_mid);



		if (!d.my_units.empty())
			head_unit = d.my_units.front()->id;
		else
			head_unit = d.my_tiles.front()->id;


		// debug("first_unit" , d.tiles.at(first_unit));
		debug("head_unit" , d.tiles.at(head_unit));
		// debug("last_unit" , d.tiles.at(last_unit));



// spread
		// rearrangement des voisins de mes premiers robots
		for (auto& my_unit : d.my_units)
		{
			Tile* closest_mid_to_my_unit = d.closestMidTileTo(*my_unit);

			std::vector<Tile *> neighbors = my_unit->getNeighbors();

			if (my_unit->id == last_unit || my_unit->id == first_unit) //test
			{
				std::vector<Tile *> tmp(neighbors);
				neighbors.clear();
				for(auto it = tmp.begin(); it != tmp.end(); it++)
				{
					if ((*it)->x == my_unit->x)
					{
						neighbors.push_back(*it);
						// debug("x", **it);

					}
				}
				for(auto it = tmp.begin(); it != tmp.end(); it++)
				{
					if ((*it)->y == my_unit->y)
					{
						neighbors.push_back(*it);
						// debug("y", **it);

					}
				}
			}
			int _rand = std::rand()/((RAND_MAX)/height);
			debug("rand =", _rand);
			if (_rand > my_unit->y /*my_unit->id == head_unit && my_unit->y > height / 2*/)
			{
				auto it1 = neighbors.begin();
				while (it1 != neighbors.end() && (**it1).x != my_unit->x)
				{
					it1++;
				}
				auto it2 = neighbors.begin();
				while (it2 != neighbors.end() && ( (**it2).x != my_unit->x || *it2 == *it1 ) )
				{
					it2++;
				}
				if (it1 != neighbors.end() && it2 != neighbors.end())
					std::swap(*it1, *it2);
			}
			
//debut spread
			for (auto& neighbor : neighbors)
			{
				debug("neighbor :", *neighbor);
				if (is_neutral(*neighbor))
				{
					debug("neighbor :", *neighbor);

					Tile* closest_mid_to_neighbor = d.closestMidTileTo(*neighbor);
					if (closest_mid_to_neighbor == nullptr)
						continue ;
					
					debug("closest_mid :", *closest_mid_to_neighbor);

					debug(closest_mid_to_neighbor->dist_to_start - closest_mid_to_neighbor->getDistanceTo(*neighbor) - 1,
						closest_mid_to_my_unit->dist_to_start - closest_mid_to_my_unit->getDistanceTo(*my_unit));


					// if (closest_mid_to_neighbor->getDistanceTo(*neighbor) < closest_mid_to_neighbor->dist_to_start - turn)
					// if (d.closestMidTileTo(*neighbor)->getDistanceTo(*neighbor) <=
					// 	d.closestMidTileTo(*my_unit)->getDistanceTo(*my_unit) )
					if (closest_mid_to_neighbor->dist_to_start - closest_mid_to_neighbor->getDistanceTo(*neighbor) -1 >=
						closest_mid_to_my_unit->dist_to_start - closest_mid_to_my_unit->getDistanceTo(*my_unit))
					{
						
						// debug("doit etre egal à -turn pour spawn : ", my_unit->getDistanceTo(*closest_mid_to_my_unit) - closest_mid_to_my_unit->dist_to_start + 1);
						if (my_unit->getDistanceTo(*closest_mid_to_my_unit) - closest_mid_to_my_unit->dist_to_start + 1 == -turn)
						{
							if (my_unit->units >= 1)
							{
								debug("move");
								my_unit->move(1, *neighbor);
								my_unit->units--;
								neighbor->owner = TARGETED;
								closest_mid_to_neighbor->owner = TARGETED;
							}
							else if (d.my_matter >= 10)
							{
								debug("spawn");
								d.spawn(*my_unit, 1);
								neighbor->owner = TARGETED;
								closest_mid_to_neighbor->owner = TARGETED;
							}
						}
						else if (my_unit->units >= 1)
							{
								debug("move");
								my_unit->move(1, *neighbor);
								my_unit->units--;
								neighbor->owner = TARGETED;
								closest_mid_to_neighbor->owner = TARGETED;
							}

					}

				}
			}
		} // temps courant, apres l'execution
   
// fin spread

		// while (d.my_matter >= 10)
		// {
		// 	// for (auto& my_tile : d.getTilesIf(make_is_matching))
		// 	d.spawn(*d.my_tiles[0], d.my_matter / 10);
		// }

// pour chaque unités restantes
		for (auto& my_unit : d.my_units)
		{
			if (my_unit->units >= 1)
			{
				if (my_unit->isolated)
				{
					Tile* closest = d.getClosest(*my_unit, is_neutral);
					if (closest)
					{
						my_unit->move(my_unit->units, *closest);
						my_unit->units = 0;
					}
					
				}
				else
				{
					// for (auto& neighbor : my_unit->getNeighbors(is_neutral, 0))
					// {
					// 	my_unit->move(1, *neighbor);
					// 	my_unit->units--;
					// }
					if (my_unit->units >= 1)
					{
						Tile* closest = d.getClosest(*my_unit, is_opp);
						if (closest)
						{
							my_unit->move(my_unit->units, *closest);
							my_unit->units = 0;
						}
					}
				}

			}
		}

// attaque si il reste de la moula
		bool	did_something = 1;
		if (d.my_matter >= 10 && did_something)
		{
			did_something = 0;
			for (auto& _opp_tile : d.opp_tiles)
			{
				if (d.my_matter < 10)
					break;
				Tile& opp_tile = *_opp_tile;
				if (is_recycler(opp_tile))
					continue;
				for (auto& neighbor : opp_tile.getNeighbors())
				{
					if (d.my_matter < 10)
						break;
					if ( ! is_me(*neighbor) || is_recycler(*neighbor) || neighbor->def_units <= 0)
						continue ;
					Tile& my_tile = *neighbor;
					d.spawn(my_tile, 1);
					my_tile.move(1, opp_tile);
					my_tile.def_units--;
					did_something = 1;
				}
				if (d.my_matter < 10)
					break;

				for (auto& neighbor : opp_tile.getNeighbors())
				{
					if (d.my_matter < 10)
						break;
					if ( ! is_me(*neighbor) || is_recycler(*neighbor))
						continue ;
					Tile& my_tile = *neighbor;
					d.spawn(my_tile, 1);
					did_something = 1;
				}
			}
		}

		if (d.my_matter >= 10)
		{
			for (auto& _my_tile : d.my_tiles)
			{
				if (d.my_matter < 10)
					break ;
				Tile& my_tile = *_my_tile;
				if (my_tile.isolated && !is_unit(my_tile) && !is_recycler(my_tile))
				{
					Tile* neutral = d.getClosest(my_tile, is_neutral);
					Tile* unit = d.getClosest(my_tile, is_unit);
					if (neutral && !unit)
						d.spawn(my_tile, 1);
				}
			}
		}



		message("🦊");

		nb_bfs();
// temps courant, apres l'execution
std::chrono::high_resolution_clock::time_point b= std::chrono::high_resolution_clock::now();

// mesurer la difference, et l'exprimer en microsecondes 
float time= (float)std::chrono::duration_cast<std::chrono::microseconds>(b - a).count() / 1000;

	std::cerr << "temps d exec : " << time << " ms" << std::endl;
		std::cout << "WAIT;" << std::endl;
		turn++;

	}// while(1)
}
