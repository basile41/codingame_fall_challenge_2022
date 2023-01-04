# include "includes.hpp"

# include "Graph.hpp"
# include "Data.hpp"

static int compteur_bfs = 0;

int bfs(Graph &graph, int startId)
{
	// Initialize a queue for BFS and a set to store visited vertices
	std::queue<Vertex*> q;

	// Find the starting vertex and add it to the queue

	graph.visited.clear();
	Vertex *start = graph.findVertex(startId);
	if (!start)
	{
		std::cerr << "id introuvable" << std::endl;
		return (-1);
	}
	compteur_bfs++;
	start->distance = 0;
	q.push(start);
	graph.visited.insert(startId);

	while (!q.empty())
	{
		// Get the next vertex in the queue and visit it
		Vertex *v = q.front();
		q.pop();
		// std::cerr << "Visiting vertex " << v->id << std::endl;
		// std::cerr << "distance =  " << v->distance << std::endl;
		// std::cerr << "id " << graph.tiles->at(v->id).id << std::endl;
		// std::cerr << "owner " << graph.tiles->at(v->id).owner << std::endl;

		// Add all of its unvisited neighbors to the queue
		for (Edge &e : v->edges)
		{
			Vertex *to = e.to;
			if (graph.visited.find(to->id) == graph.visited.end())
			{
				to->distance = v->distance + 1;
				q.push(to);
				graph.visited.insert(to->id);
			}
		}
	}
	return (-1);
}

int bfs(Graph &graph, int startId, TileCondition is_matching)
{
	std::queue<Vertex*> q;


	graph.visited.clear();
	Vertex *start = graph.findVertex(startId);
	if (!start)
	{
		std::cerr << "id introuvable" << std::endl;
		return (-1);
	}
	compteur_bfs++;
	start->distance = 0;
	q.push(start);
	graph.visited.insert(startId);

	while (!q.empty())
	{
		// Get the next vertex in the queue and visit it
		Vertex *v = q.front();
		q.pop();
		// std::cerr << "Visiting vertex " << v->id << std::endl;
		// std::cerr << "distance =  " << v->distance << std::endl;
		// std::cerr << "id " << graph.tiles->at(v->id).id << std::endl;
		// std::cerr << "owner " << graph.tiles->at(v->id).owner << std::endl;
		if (is_matching(graph.tiles->at(v->id)))
		{
			return (v->id);
		}

		// Add all of its unvisited neighbors to the queue
		for (Edge &e : v->edges)
		{
			Vertex *to = e.to;
			if (graph.visited.find(to->id) == graph.visited.end())
			{
				to->distance = v->distance + 1;
				q.push(to);
				graph.visited.insert(to->id);
			}
		}
	}
	return (-1);
}

int bfs(Graph &graph, int startId, int targetId, TileCondition is_matching)
{
	int dist_to_target = graph.tiles->at(startId).distances.at(targetId);
	// Initialize a queue for BFS and a set to store visited vertices
	std::queue<Vertex*> q;

	// Find the starting vertex and add it to the queue

	graph.visited.clear();
	Vertex *start = graph.findVertex(startId);
	if (!start)
	{
		std::cerr << "id introuvable" << std::endl;
		return (-1);
	}
	compteur_bfs++;
	start->distance = 0;
	q.push(start);
	graph.visited.insert(startId);

	while (!q.empty())
	{
		// Get the next vertex in the queue and visit it
		Vertex *v = q.front();
		q.pop();
		// std::cerr << "Visiting vertex " << v->id << std::endl;
		// std::cerr << "distance =  " << v->distance << std::endl;
		// std::cerr << "id " << graph.tiles->at(v->id).id << std::endl;
		// std::cerr << "owner " << graph.tiles->at(v->id).owner << std::endl;
		if (is_matching(graph.tiles->at(v->id)))
		{
			if (v->distance + graph.tiles->at(v->id).distances.at(targetId) == dist_to_target) // si la case trouvé me rapproche de la cible
				return (v->id);
		}

		// Add all of its unvisited neighbors to the queue
		for (Edge &e : v->edges)
		{
			Vertex *to = e.to;
			if (graph.visited.find(to->id) == graph.visited.end())
			{
				to->distance = v->distance + 1;
				q.push(to);
				graph.visited.insert(to->id);
			}
		}
	}
	return (-1);
}

// a partir de plusieurs tiles
int bfs_multi_start(Graph &graph, TileCondition is_start)
{
	std::queue<Vertex*> q;


	graph.visited.clear();
	for (auto& tile : *graph.tiles)
	{
		if (is_start(tile))
		{
			Vertex *start = graph.findVertex(tile.id);
			q.push(start);
			start->distance = 0;
			graph.visited.insert(tile.id);
		}
	}
	if (q.empty())
	{
		std::cerr << "starts introuvable" << std::endl;
		return (-1);
	}
	compteur_bfs++;
	
	while (!q.empty())
	{
		// Get the next vertex in the queue and visit it
		Vertex *v = q.front();
		q.pop();
		std::cerr << "Visiting vertex " << v->id << std::endl;
		std::cerr << "distance =  " << v->distance << std::endl;
		std::cerr << "coord " << graph.tiles->at(v->id) << std::endl;
		// std::cerr << "owner " << graph.tiles->at(v->id).owner << std::endl;

		// Add all of its unvisited neighbors to the queue
		for (Edge &e : v->edges)
		{
			Vertex *to = e.to;
			if (graph.visited.find(to->id) == graph.visited.end())
			{
				to->distance = v->distance + 1;
				q.push(to);
				graph.visited.insert(to->id);
			}
		}
	}
	return (-1);
}

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
			// if (&tile == d.my_units.back()) //test
			// {
			// 	std::vector<Tile *> tmp = tile.getNeighbors(make_is_matching(is_neutral, is_walkable));
			// 	std::vector<Tile *> neighbors;
			// 	for(auto it = tmp.begin(); it != tmp.end(); it++)
			// 	{
			// 		if ((*it)->x == tile.x)
			// 			neighbors.push_back(*it);
			// 	}
			// 	for(auto it = tmp.begin(); it != tmp.end(); it++)
			// 	{
			// 		if ((*it)->y == tile.y)
			// 			neighbors.push_back(*it);
			// 	}

			// 	for (auto &neighbor : neighbors)
			// 	{
			// 		graph.addEdge(tile.id, neighbor->id);
			// 	}
			// }

			for (auto &neighbor : tile.getNeighbors(make_is_matching(is_walkable, is_not(is_almost_grass)), is_neutral))
			{
				graph.addEdge(tile.id, neighbor->id);
			}

			// for (auto &neighbor : tile.getNeighbors(make_is_matching(is_neutral, is_walkable)))
			// {
			// 	graph.addEdge(tile.id, neighbor->id);
			// }
			// for (auto &neighbor : tile.getNeighbors(make_is_matching(is_not(is_neutral), is_walkable)))
			// {
			// 	graph.addEdge(tile.id, neighbor->id);
			// }
		}
	}
}

void nb_bfs()
{
	std::cerr << "nombre de bfs : " << compteur_bfs << std::endl;
	compteur_bfs = 0;
}
