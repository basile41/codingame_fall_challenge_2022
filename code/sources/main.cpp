
#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"


void	move_support_from_neighbor(Tile& tile, int required)
{
	auto neighbors = tile.getNeighbors(is_me);

	for (auto& neighbor : tile.getNeighbors(is_me)) // move les renforts voisns
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
	int		turn;
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
		else
			d.setMidTilesMulti();


		debug("mid_tiles :");
		for (auto& mid_tile : d.mid_tiles)
		{
			debug(*mid_tile, mid_tile->dist_to_start);
		}



		// test voisins prioritaires
		

// defini mes territoires isolés
		for (auto &my_tile : d.my_tiles)
		{
			if (!my_tile->recycler && !my_tile->isolated)
			{
				if (bfs(graph, my_tile->id, is_opp) == -1)
				{
					for (auto& visited : graph.visited)
					{
						debug("me :", d.tiles[visited]);
						d.tiles[visited].isolated = true;
					}
				}
			}
		}

// defini territoires opp isolés
		for (auto &opp_tile : d.opp_tiles)
		{
			if (!opp_tile->recycler && !opp_tile->isolated)
			{
				if (bfs(graph, opp_tile->id, is_me) == -1)
				{
					for (auto& visited : graph.visited)
					{
						d.tiles[visited].isolated = true;
					}
				}
			}
		}

		// test new bfs
		// for (auto& visited : graph.visited)
		// {
		// 	d.tiles[visited]
		// }


		// test
		int my_value = d.my_matter;
		int opp_value = d.opp_matter;
		for (auto& tile : d.tiles)
		{
			if (!tile.isolated)
			{
				if (is_me(tile))
					my_value += 10 * tile.units;
				if (is_opp(tile))
					opp_value += 10 * tile.units;
			}
			my_value += tile.getRecyclingValue(ME);
			opp_value += tile.getRecyclingValue(OPP);
		}
		debug("my_value :", my_value);
		debug("opp_value :", opp_value);


// defense
		// pour chacune de mes tuiles
		for (auto& _my_tile : d.my_tiles) 
		{
			Tile &	my_tile = *_my_tile;

			if (is_recycler(my_tile))
				continue ;
			if (my_tile.isNextTo( make_is_matching(is_opp, is_unit) ) && !is_almost_grass(my_tile))
			{
				// d.lost_tiles_mult = 20;
				if (DEBUG_DEF)debug("my_tile", my_tile);
				int opp_neighbor_units = my_tile.countNeighborsUnits(OPP);
				int required = opp_neighbor_units - my_tile.units;
				if (my_tile.can_build && !my_tile.isNextTo(is_neutral))
				{
					if (DEBUG_DEF)debug("can_build");
					if (0)// si valable de build -> build (à définir)
					{}
					else if (my_tile.potentialSupport() >= opp_neighbor_units)
					{
						move_support_from_neighbor(my_tile, opp_neighbor_units);
					}
					else if (d.my_matter >= 10)
					{
						if (required == 1 && (my_value / 10 >= opp_value / 10))
						{
							d.spawn(my_tile, 1);
							my_tile.def_units = my_tile.units;
							my_tile.units = 0;
						}
						else
							d.build(my_tile);
					}
				}
				if (DEBUG_DEF)debug("my_tile.units", my_tile.units);
				if (DEBUG_DEF)debug("my_tile.def_units", my_tile.def_units);
			}
		}
		d.setMidTilesMulti();

		
// defini mes territoires isolés
		for (auto &my_tile : d.my_tiles)
		{
			if (!my_tile->recycler && !my_tile->isolated)
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

// defini territoires opp isolés
		for (auto &opp_tile : d.opp_tiles)
		{
			if (!opp_tile->recycler && !opp_tile->isolated)
			{
				if (bfs(graph, opp_tile->id, is_me) == -1)
				{
					for (auto& visited : graph.visited)
					{
						d.tiles[visited].isolated = true;
					}
				}
			}
		}


// defense
		// pour chacune de mes tuiles
		for (auto& _my_tile : d.my_tiles) 
		{
			Tile &	my_tile = *_my_tile;

			if (is_recycler(my_tile))
				continue ;
			if (my_tile.isNextTo( make_is_matching(is_opp, is_unit) ) && !is_almost_grass(my_tile))
			{
				// d.lost_tiles_mult = 20;
				if (DEBUG_DEF)debug("my_tile", my_tile);
				int opp_neighbor_units = my_tile.countNeighborsUnits(OPP);
				int required = opp_neighbor_units - my_tile.units;
				if (my_tile.can_build && !my_tile.isNextTo(is_neutral))
				{
					if (DEBUG_DEF)debug("can_build");
					if (0)// si valable de build -> build (à définir)
					{}
					else if (my_tile.potentialSupport() >= opp_neighbor_units)
					{
						move_support_from_neighbor(my_tile, opp_neighbor_units);
					}
					else if (d.my_matter >= 10)
					{
						if (required == 1 && (my_value / 10 >= opp_value / 10))
						{
							d.spawn(my_tile, 1);
							my_tile.def_units = my_tile.units;
							my_tile.units = 0;
						}
						else
							d.build(my_tile);
					}
				}
				else if (my_tile.potentialSupport() + my_tile.units >= opp_neighbor_units)
				{
					int old_units = my_tile.units;
					if (DEBUG_DEF)debug("move support");
					move_support_from_neighbor(my_tile, required);

					my_tile.units = std::max(_my_tile->units - opp_neighbor_units, 0);
					my_tile.def_units = old_units - my_tile.units;

				}
				else if (d.my_matter >= 10)
				{
					// int old_units = my_tile.units;
					if (DEBUG_DEF)debug("move support et spawn");
					move_support_from_neighbor(my_tile, required);
					// d.spawn(my_tile, required);
					d.spawn(my_tile, 1);
					my_tile.def_units = my_tile.units;
					my_tile.units = 0;
				}
				if (DEBUG_DEF)debug("my_tile.units", my_tile.units);
				if (DEBUG_DEF)debug("my_tile.def_units", my_tile.def_units);
			}
			// if (d.my_matter >= 10 && my_tile.isNextTo(is_neutral) && my_tile.def_units >= 1)
			// {
			// 	d.spawn(my_tile, 1);
			// 	my_tile.units++;
			// 	my_tile.def_units--;
			// }
		}
		
// defini mes territoires isolés
		for (auto &my_tile : d.my_tiles)
		{
			if (!my_tile->recycler && !my_tile->isolated)
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

// defini territoires opp isolés
		for (auto &opp_tile : d.opp_tiles)
		{
			if (!opp_tile->recycler && !opp_tile->isolated)
			{
				if (bfs(graph, opp_tile->id, is_me) == -1)
				{
					for (auto& visited : graph.visited)
					{
						d.tiles[visited].isolated = true;
					}
				}
			}
		}


		// // test spawn proche ennemi
		// for (auto& my_tile : d.my_tiles)
		// {
		// 	if (!my_tile->isNextTo(make_is_matching(is_me, is_unit)))
		// 	{
		// 		debug("my_unit :", *my_tile);
		// 		for (auto& neighbor : my_tile->getNeighbors(make_is_matching(is_neutral)))
		// 		{
		// 			debug("neighbor :", *neighbor);
					
		// 			if(neighbor->isNextTo(is_opp))
		// 			{
		// 				debug("spawn");
		// 				d.spawn(*my_tile, 1);
		// 				break ;
		// 			}
		// 		}

		// 	}

		// }

		// if ((my_value / 10 < opp_value / 10))
		// 	d.lost_tiles_mult = 5;

		// d.lost_tiles_mult = std::max(0, 10 + (my_value - opp_value) / 5);

// build investisssement (à ameliorer)
		if (d.my_matter >= 10)
		{
			int max_rent = 0;
			int max_rent_id;
			for (auto& my_tile : d.my_tiles)
			{
				if (my_tile->can_build && my_tile->units == 0 && !my_tile->isolated && !is_recycler(*my_tile))
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
			if (!my_tile->recycler && !my_tile->isolated)
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

// defini territoires opp isolés
		for (auto &opp_tile : d.opp_tiles)
		{
			if (!opp_tile->recycler && !opp_tile->isolated)
			{
				if (bfs(graph, opp_tile->id, is_me) == -1)
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

// set les unités primaires
		int first_unit = -1;
		int head_unit = -1;
		int last_unit = -1;

		std::vector<Tile*> parents;
		for (auto& my_unit : d.my_tiles)
		{
			Tile* closest_mid_to_my_unit = d.closestMidTileTo(*my_unit);
			if (closest_mid_to_my_unit == nullptr)
				break;
			if (my_unit->getDistanceTo(*closest_mid_to_my_unit) - closest_mid_to_my_unit->dist_to_start + 1 == 0)
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

			if (DEBUG_SPREAD)debug("first_unit" , d.tiles.at(first_unit));
			if (DEBUG_SPREAD)debug("head_unit" , d.tiles.at(head_unit));
			if (DEBUG_SPREAD)debug("last_unit" , d.tiles.at(last_unit));

		}
		else if (parents.size() == 2)
		{
			TilePtrCompare comp = [=](Tile *t1, Tile *t2)->bool
			{
				if (d.my_side)
					return (t1->x > t2->x);
				return (t1->x < t2->x);
			};
			std::sort(parents.begin(), parents.end(), comp);
			head_unit = parents[0]->id;
			if (DEBUG_SPREAD)debug("head_unit" , d.tiles.at(head_unit));

			if (parents[1]->y < parents[0]->y)
			{
				first_unit = parents[1]->id;
				if (DEBUG_SPREAD)debug("first_unit" , d.tiles.at(first_unit));
			}
			else
			{
				last_unit = parents[1]->id;
				if (DEBUG_SPREAD)debug("last_unit" , d.tiles.at(last_unit));
			}

		}
		else if (parents.size() == 1)
		{
			head_unit = parents[0]->id;
			if (DEBUG_SPREAD)debug("head_unit" , d.tiles.at(head_unit));
		}
		std::sort(d.my_units.begin(), d.my_units.end(), comp_dist_to_mid);
		std::sort(d.my_tiles.begin(), d.my_tiles.end(), comp_dist_to_mid);

		std::sort(d.my_tiles.begin(), d.my_tiles.end(), [&](Tile* t1, Tile* t2)->bool
		{
			Tile* c1 = d.closestMidTileTo(*t1);
			Tile* c2 = d.closestMidTileTo(*t2);
			if (c1 == nullptr || c2 == nullptr)
				return false;
			int d1 = t1->getDistanceTo(*c1) - c1->dist_to_start + 1;
			int d2 = t2->getDistanceTo(*c2) - c2->dist_to_start + 1;
			if (d1 == 0 && d2 != 0)
				return true;
			return false;

			
			// Tile* c1 = d.closestMidTileTo(*t1);
			// Tile* c2 = d.closestMidTileTo(*t2);
			// if (!c1 || !c2)
			// 	return false;
			// return (c1->dist_to_start < c2->dist_to_start);
		});


		for (auto& mid_tile : d.mid_tiles)
		{
			if (DEBUG_SPREAD)debug(*mid_tile, mid_tile->dist_to_start);
		}

// spread

		// std::random_shuffle(d.my_tiles.begin(), d.my_tiles.begin() + parents.size());

		std::sort(d.my_tiles.begin(), d.my_tiles.begin() + parents.size(), [&](Tile* t1, Tile* t2)->bool
		{
			int neighbors_units1 = 0;
			int neighbors_units2 = 0;
			for (auto& nei : t1->getNeighbors(make_is_matching(is_me, is_unit)))
				neighbors_units1 += nei->units;
			debug("t1:");
			debug(*t1, neighbors_units1);
			for (auto& nei : t2->getNeighbors(make_is_matching(is_me, is_unit)))
				neighbors_units2 += nei->units;
			debug("t2:");
			debug(*t2, neighbors_units2);
			return neighbors_units1 < neighbors_units2;
		});

		// std::random_shuffle(d.my_units.begin(), d.my_units.begin() + parents.size());
		// if (d.my_side_y)
		// {
		// 	TilePtrCompare comp = [](Tile* t1, Tile* t2)->bool
		// 	{
		// 		return (t1->y < t2->y);
		// 	};
		// 	std::sort(d.my_tiles.begin(), d.my_tiles.end(), comp);
		// 	std::sort(d.my_units.begin(), d.my_units.end(), comp);
		// }
		// else
		// {
		// 	TilePtrCompare comp = [](Tile* t1, Tile* t2)->bool
		// 	{
		// 		return (t1->y > t2->y);
		// 	};
		// 	std::sort(d.my_tiles.begin(), d.my_tiles.end(), comp);
		// 	std::sort(d.my_units.begin(), d.my_units.end(), comp);
		// }


		for (auto& my_unit : d.my_tiles)
		{
// debug("my_unit", *my_unit);
			Tile*	closest_mid_to_my_unit = d.closestMidTileTo(*my_unit);
// debug("apres");

			if (DEBUG_SPREAD)debug("my_unit :", *my_unit);


			std::vector<Tile *> neighbors = my_unit->getNeighbors(is_neutral);
			// if (my_unit->y > height / 2)
			// {
			// 	TilePtrCompare comp = [](Tile* t1, Tile* t2)->bool
			// 	{
			// 		return (t1->y < t2->y);
			// 	};
			// 	std::sort(neighbors.begin(), neighbors.end(), comp);
			// }

			// tri des voisins
			std::sort(neighbors.begin(), neighbors.end(), comp_dist_to_mid);

			if (my_unit->id == last_unit)
			{
				// if (d.circular_symmetry)
				// {
				// 	neighbors.clear();
				// 	if (my_unit->bottom)
				// 		neighbors.push_back(my_unit->bottom);
				// 	if (my_unit->left)
				// 		neighbors.push_back(my_unit->left);
				// 	if (my_unit->right)
				// 		neighbors.push_back(my_unit->right);
				// 	if (my_unit->top)
				// 		neighbors.push_back(my_unit->top);

				// }
			}
			else if (my_unit->id == first_unit)
			{
				// if (d.circular_symmetry)
				// {
				// 	neighbors.clear();
				// 	if (my_unit->top)
				// 		neighbors.push_back(my_unit->top);
				// 	if (my_unit->left)
				// 		neighbors.push_back(my_unit->left);
				// 	if (my_unit->right)
				// 		neighbors.push_back(my_unit->right);
				// 	if (my_unit->bottom)
				// 		neighbors.push_back(my_unit->bottom);
					
				// }
			}
			else if (my_unit->id == head_unit)
			{
				// std::sort(neighbors.begin(), neighbors.end(), comp_dist_to_mid);
			}
			// if (my_unit->id == last_unit || my_unit->id == first_unit) //test
			// {
			// 	std::vector<Tile *> tmp(neighbors);
			// 	neighbors.clear();
			// 	for(auto it = tmp.begin(); it != tmp.end(); it++)
			// 	{
			// 		if ((*it)->x == my_unit->x)
			// 			neighbors.push_back(*it);
			// 	}
			// 	for(auto it = tmp.begin(); it != tmp.end(); it++)
			// 	{
			// 		if ((*it)->y == my_unit->y)
			// 			neighbors.push_back(*it);
			// 	}
			// }
			else
			{
				if (neighbors.size() == 2)
				{
					if (neighbors[0]->getNeighbors(is_me).size() >= 2)
					{
						if (neighbors[1]->getNeighbors(is_me).size() == 1)
							std::swap(neighbors[0], neighbors[1]);
						else if (my_unit->y > height / 2)
						{
							TilePtrCompare comp = [](Tile* t1, Tile* t2)->bool
							{
								return (t1->y < t2->y);
							};
							std::sort(neighbors.begin(), neighbors.end(), comp);
						}
					}
				}
			}
			
			// debut spread
			for (auto& neighbor : neighbors)
			{
				if (is_neutral(*neighbor))
				{
					if (DEBUG_SPREAD)debug("neighbor :", *neighbor);

					Tile* closest_mid_to_neighbor = d.closestMidTileTo(*neighbor);
					if (closest_mid_to_neighbor == nullptr)
						continue ;
					
					if (DEBUG_SPREAD)debug("closest_mid :", *closest_mid_to_neighbor);

					// debug(closest_mid_to_neighbor->dist_to_start - closest_mid_to_neighbor->getDistanceTo(*neighbor) -1,
					// 	closest_mid_to_my_unit->dist_to_start - closest_mid_to_my_unit->getDistanceTo(*my_unit));

					// if (closest_mid_to_neighbor->dist_to_start - closest_mid_to_neighbor->getDistanceTo(*neighbor) -1 >=
					// 	closest_mid_to_my_unit->dist_to_start - closest_mid_to_my_unit->getDistanceTo(*my_unit))

					// debug(d.getClosest(*neighbor, is_mid_tile)->getDistanceTo(*neighbor),
					// 	d.getClosest(*my_unit, is_mid_tile)->getDistanceTo(*my_unit));

					// if (d.getClosest(*neighbor, is_mid_tile)->getDistanceTo(*neighbor) <=
					// 	d.getClosest(*my_unit, is_mid_tile)->getDistanceTo(*my_unit))
					// if (neighbor->isNearestThan(*my_unit, is_mid_tile))
					if (neighbor->isNearestToMid(*my_unit))
					{
						if (DEBUG_SPREAD)debug(*neighbor, "is nearest");
						if (DEBUG_SPREAD)debug("doit etre egal à turn pour spawn : ", my_unit->getDistanceTo(*closest_mid_to_my_unit) - closest_mid_to_my_unit->dist_to_start + 1);
						if (my_unit->getDistanceTo(*closest_mid_to_my_unit) - closest_mid_to_my_unit->dist_to_start + 1 == 0)
						{
							if (DEBUG_SPREAD)debug("my_unit->def_units :", my_unit->def_units);
							if (my_unit->units == 0 && my_unit->def_units >= 1 && d.my_matter >= 10)
							{
								d.spawn(*my_unit, 1);
								my_unit->def_units--;
								my_unit->units++;
							}
							if (DEBUG_SPREAD)debug("my_unit->units :", my_unit->units);

							if (my_unit->units >= 1)
							{
								if (DEBUG_SPREAD)debug("move");
								my_unit->move(1, *neighbor);
								my_unit->units--;
								neighbor->owner = TARGETED;
								// closest_mid_to_neighbor->owner = TARGETED;
							}
							else if (d.my_matter >= 10 && !neighbor->isNextTo(make_is_matching(is_me, is_unit)))
							{
								if (DEBUG_SPREAD)debug("spawn");
								d.spawn(*my_unit, 1);
								neighbor->owner = TARGETED;
								// closest_mid_to_neighbor->owner = TARGETED;
							}
						}
						else if (DEBUG_SPREAD)debug("my_unit->def_units :", my_unit->def_units);
						if (my_unit->units == 0 && my_unit->def_units >= 1 && d.my_matter >= 10)
						{
							d.spawn(*my_unit, 1);
							my_unit->def_units--;
							my_unit->units++;
						}
						if (my_unit->units >= 1)
						{
							if (DEBUG_SPREAD)debug("move");
							my_unit->move(1, *neighbor);
							my_unit->units--;
							neighbor->owner = TARGETED;
							// closest_mid_to_neighbor->owner = TARGETED;
						}

					}

				}
			}
// en cours
			// if (my_unit->units >= 1)
			// {
			// 	auto neighbors = my_unit->getNeighbors(is_walkable, [&](Tile& tile)->bool
			// 	{
			// 		return (tile.x == my_unit->x);
			// 	});
			// 	for (auto& neighbor : neighbors)
			// 	{
			// 		if (!is_almost_grass(*neighbor) && neighbor->isNearestToMid(*my_unit) && !my_unit->isolated)
			// 		{
			// 			debug(*neighbor, "isNearestToMid (apres spread)");
			// 			my_unit->move(my_unit->units, *neighbor);
			// 			my_unit->units = 0;
			// 		}

			// 	}
				

			// }

		}

// fin spread

		// while (d.my_matter >= 10)
		// {
		// 	// for (auto& my_tile : d.getTilesIf(make_is_matching))
		// 	d.spawn(*d.my_tiles[0], d.my_matter / 10);
		// }

		for (auto& my_unit : d.my_units)
		{
			debug(*my_unit, my_unit->units);
		}

// pour chaque unités restantes
		for (auto& my_unit : d.my_units)
		{
			if (my_unit->units >= 1)
			{
				if (my_unit->isolated) // si isolé
				{
					Tile* closest = d.getClosest(*my_unit, is_neutral);
					if (closest)
					{
						my_unit->move(my_unit->units, *closest);
						// my_unit->units = 0;
					}
					
				}
				else
				{
					// for (auto& neighbor : my_unit->getNeighbors(is_neutral))
					// {
					// 	my_unit->move(1, *neighbor);
					// 	my_unit->units--;
					// }
					if (my_unit->units >= 1)
					{
						Tile* closest = d.getClosest(*my_unit, make_is_matching(is_opp, is_not(is_almost_grass)));
						// if (!closest)
						// 	closest = d.getClosest(*my_unit, is_opp);
						if (closest)
						{
							int dist_to_ennemy = my_unit->getDistanceTo(*closest);
							for (auto& neighbor : my_unit->getNeighbors(make_is_matching(is_walkable, is_not(is_almost_grass)), is_neutral))
							{
								if (neighbor->getDistanceTo(*closest) < dist_to_ennemy)
								{
									my_unit->move(my_unit->units, *neighbor);
									my_unit->units = 0;
									break;
								}
							}

						}
					}
				}

			}
		}


		for (auto& my_unit : d.my_units)
		{
			debug(*my_unit, my_unit->units);
		}

// attaque si il reste de la moula
		bool	did_something = 1;
		while (d.my_matter >= 10 && did_something)
		{
			did_something = 0;
			for (auto& _opp_tile : d.opp_tiles)
			{
				if (d.my_matter < 10)
					break;
				Tile& opp_tile = *_opp_tile;
				if (is_almost_grass(opp_tile))
					continue ;
				if (is_recycler(opp_tile))
					continue;
				for (auto& neighbor : opp_tile.getNeighbors())
				{
					if (d.my_matter < 10)
						break;
					if ( ! is_me(*neighbor) || is_recycler(*neighbor) || neighbor->def_units <= 0 || is_almost_grass(*neighbor))
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
					if ( ! is_me(*neighbor) || is_recycler(*neighbor) || is_almost_grass(*neighbor))
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
					{
						// debug("COUCOU");
						d.spawn(my_tile, 1);
						my_tile.units = 1;
					}
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
