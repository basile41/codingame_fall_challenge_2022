#include "includes.hpp"
#include "Data.hpp"

Data::Data(int width, int height)
: width(width), height(height), size(width * height), turn(-1), tiles(size, Tile(size))
{
	int id = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++, id++)
		{
			Tile* tile = &tiles[id];
			tile->id = id;
			tile->x = x;
			tile->y = y;
			if (x > 0)
			{
				Tile* left = tile - 1;
				tile->left = left;
				left->right = tile;
			}
			if (y > 0)
			{
				Tile* top = tile - width;
				tile->top = top;
				top->bottom = tile;
			}

		}
	}
}

void Data::read()
{
	turn++;
	int id = 0;
	std::cin >> my_matter >> opp_matter;
	std::cin.ignore();
	my_tiles.clear();
	opp_tiles.clear();
	neutral_tiles.clear();
	my_units.clear();
	opp_units.clear();
	my_recyclers.clear();
	opp_recyclers.clear();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++, id++)
		{
			Tile* tile = &tiles[id];
			tile->read();
			tile->targeted = false;
			tile->def_units = 0;
			if (tile->owner == ME)
			{
				my_tiles.push_back(tile);
				if (tile->units > 0)
				{
					my_units.push_back(tile);
				}
				else if (tile->recycler)
				{
					my_recyclers.push_back(tile);
				}
			}
			else if (tile->owner == OPP)
			{
				opp_tiles.push_back(tile);
				if (tile->units > 0)
				{
					opp_units.push_back(tile);
				}
				else if (tile->recycler)
				{
					opp_recyclers.push_back(tile);
				}
			}
			else
			{
				neutral_tiles.push_back(tile);
			}
		}
	}
	
}

void	Data::setAllDistance()
{
	for (auto& tile : tiles)
		{
			if (tile.scrap_amount && !tile.recycler)
			{
				for (auto& vertex : graph->vertices)
					vertex.distance = 999;
				bfs(*graph, tile.id);
				for (auto& vertex : graph->vertices)
				{
					tile.distances[vertex.id] = vertex.distance;
				}
			}
		}
}

void	Data::setMidTiles()
{
	dist_start_to_center = 999;
	Tile& my_start = *my_tiles[2];
	Tile& opp_start = *opp_tiles[2];

	for (auto& tile : tiles)
	{
		if (is_tile(tile))
		{
			int	dist_to_my_start = my_start.getDistanceTo(tile);
			int	dist_to_opp_start = opp_start.getDistanceTo(tile);

			if (dist_to_my_start == dist_to_opp_start ||
				dist_to_my_start == dist_to_opp_start + 1 )
			{
				mid_tiles.push_back(&tile);
				tile.dist_to_start = dist_to_my_start;
				dist_start_to_center = std::min(dist_start_to_center, dist_to_my_start);
			}
		}
	}
}


Tile *	Data::getTile(int id)
{
	if (id < 0 || id >= size)
		std::cerr << "Data::getTile : Invalid ID : " << id << std::endl;
	return (&tiles[id]);
}

Tile *	Data::getTile(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return NULL;
	return (&tiles[y * width + x]);
}

std::vector<Tile*>	Data::getTilesIf(TileCondition is_matching)
{
	std::vector<Tile*> t;
	for (auto& tile : tiles)
	{
		if (is_matching(tile))
		{
			t.push_back(&tile);
		}
	}
	return t;
}

Tile *	Data::getValidTile(int x, int y)
{
	Tile* tile = getTile(x, y);
	if (!tile || tile->scrap_amount == 0)
		return NULL;
	return tile;
}

Tile *	Data::getUsableTile(int x, int y)
{
	Tile* tile = getValidTile(x, y);
	if (!tile || tile->recycler)
		return NULL;
	return tile;
}

int		Data::getId(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return -1;
	return (y * width + x);
}

std::vector<int> Data::getNeighbors(int id, bool (*f)(Tile*))
{
	std::vector<int> neighbors;
	int x = tiles.at(id).x;
	int y = tiles.at(id).y;
	for (auto &neighbor_id : {	getId(x - 1, y),
								getId(x, y - 1),
								getId(x + 1, y),
								getId(x, y + 1)})
	{
		if (neighbor_id != -1 && f(&tiles.at(neighbor_id)))
			neighbors.push_back(neighbor_id);
	}
	std::random_shuffle(neighbors.begin(), neighbors.end());
	return (neighbors);
}

std::vector<int> Data::getNeighbors(int id, bool (*f)(Tile*), int dir_x, int dir_y)
{
	std::vector<int> neighbors;
	int x = tiles.at(id).x;
	int y = tiles.at(id).y;
	for (auto &neighbor_id : {	getId(x + dir_x, y),
								getId(x, y + dir_y),
								getId(x, y - dir_y),
								getId(x - dir_x, y)})
	{
		if (neighbor_id != -1 && f(&tiles.at(neighbor_id)))
			neighbors.push_back(neighbor_id);
	}
	return (neighbors);
}

int		Data::getRecycleRent(int id)
{
	int	lost_tiles = 5;
	int scraps_amount = tiles[id].scrap_amount;
	int rent = scraps_amount;

	for (auto& neighbor : getNeighbors(id, is_tile))
	{
		if (tiles[neighbor].scrap_amount > scraps_amount)
		{
			rent += scraps_amount;
			lost_tiles--;
		}
		else
			rent += tiles[neighbor].scrap_amount;
	}
	return (rent - lost_tiles * 10);
}

int		Data::getDistance(int id1, int id2)
{
	return (tiles.at(id1).getDistanceTo(id2));
}

Tile *	Data::getClosest(Tile& start, TileCondition is_matching)
{
	int id = bfs(*graph, start.id, is_matching);
	if (id == -1)
		return (nullptr);
	return (&tiles[id]);
}

bool	Data::isFirstOfLine(Tile& my_tile, int dir_x)
{
	int y = my_tile.y;
	int border = -1;

	if (dir_x == 1)
		border = width;
	for (int x = my_tile.x + dir_x ; x != border; x += dir_x)
	{
		if (getTile(x, y)->owner == 1 && getTile(x, y)->units)
			return (false);
	}
	return (true);
}
