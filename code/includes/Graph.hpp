#ifndef GRAPH_HPP
# define GRAPH_HPP

# include "includes.hpp"

class Edge;

class Vertex
{
public:
	int	id;
	int distance;
	std::vector<Edge> edges;

	Vertex(int id) : id(id), distance(-1) {}
};

class Edge
{
public:
	Vertex *from;
	Vertex *to;

	Edge(Vertex *from, Vertex *to) : from(from), to(to) {}
};

class Graph
{
public:
	std::vector<Vertex> vertices;
	std::set<int> 		visited;

	std::vector<Tile>	*tiles;

	void addVertex(int id)
	{
		vertices.emplace_back(id);
	}

	void addEdge(int from, int to)
	{
		Vertex *v1 = findVertex(from);
		Vertex *v2 = findVertex(to);
		v1->edges.emplace_back(v1, v2);
	}

	Vertex *findVertex(int id)
	{
		for (Vertex &v : vertices)
		{
			if (v.id == id)
			{
				return &v;
			}
		}
		return nullptr;
	}
};

#endif /* GRAPH_HPP */
