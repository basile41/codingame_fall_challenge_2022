# include "includes.hpp"

# include "Graph.hpp"


void bfs(Graph &graph, int startId)
{
	// Initialize a queue for BFS and a set to store visited vertices
	std::queue<Vertex*> q;
	std::set<int> visited;

	// Find the starting vertex and add it to the queue
	Vertex *start = graph.findVertex(startId);
	start->distance = 0;
	q.push(start);
	visited.insert(startId);

	while (!q.empty())
	{
		// Get the next vertex in the queue and visit it
		Vertex *v = q.front();
		q.pop();
		std::cerr << "Visiting vertex " << v->id << std::endl;
		std::cerr << "distance =  " << v->distance << std::endl;

		// Add all of its unvisited neighbors to the queue
		for (Edge &e : v->edges)
		{
			Vertex *to = e.to;
			if (visited.find(to->id) == visited.end())
			{
				to->distance = v->distance + 1;
				q.push(to);
				visited.insert(to->id);
			}
		}
	}
}