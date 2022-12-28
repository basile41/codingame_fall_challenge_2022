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

void nb_bfs()
{
	std::cerr << "nombre de bfs : " << compteur_bfs << std::endl;
	compteur_bfs = 0;
}
