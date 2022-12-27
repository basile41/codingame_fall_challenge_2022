#ifndef INCLUDES_HPP
# define INCLUDES_HPP

// #undef _GLIBCXX_DEBUG                // disable run-time bound checking, etc
// #pragma GCC optimize("Ofast,inline") // Ofast = O3,fast-math,allow-store-data-races,no-protect-parens

// #pragma GCC target("bmi,bmi2,lzcnt,popcnt")                      // bit manipulation
// #pragma GCC target("movbe")                                      // byte swap
// #pragma GCC target("aes,pclmul,rdrnd")                           // encryption
// #pragma GCC target("avx,avx2,f16c,fma,sse3,ssse3,sse4.1,sse4.2") // SIMD

# include <bits/types/wint_t.h>
# include <cstddef>
# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <queue>
# include <set>
# include <algorithm>
# include <functional>
# include <chrono>

class Tile;
class Data;
class Graph;

# include "Tile.hpp"
# include "Graph.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

using function_is_tile = std::function<bool(Tile& t)>;

static constexpr int ME = 1;
static constexpr int OPP = 0;
static constexpr int NONE = -1;

static constexpr int LEFT = 0;
static constexpr int RIGHT = 1;



template<typename T>
void	debug(const T& value)
{
	std::cerr << value << std::endl;
};

template<typename T1, typename T2>
void	debug(const T1& value1, const T2& value2)
{
	std::cerr << value1 << " " << value2 << std::endl;
};

class Tile;

void	message(std::string message);

std::vector<Tile*> setTerritory(Tile*& s);


bool	is_tile(Tile& tile);
bool	is_me(Tile& tile);
bool	is_opp(Tile& tile);
bool	is_neutral(Tile& tile);
bool	is_unit(Tile& tile);
bool	is_recycler(Tile& tile);
bool	is_recycled_by_me(Tile& tile);
bool	is_usable_tile(Tile& tile);
bool	is_my_empty_tile(Tile& tile);
bool	is_empty_opp(Tile& tile);


bool	is_tile(Tile* tile);
bool	is_usable_tile(Tile* tile);
bool	is_my_empty_tile(Tile* tile);
bool	is_empty_opp(Tile* tile);

int		bfs(Graph &graph, int startId);
int 	bfs(Graph &graph, int startId, std::function<bool (Tile &tile)>);
int		bfs(Graph &graph, int startId, int targetId, std::function<bool (Tile &tile)> to_find);


void	init_graph(Data& d, Graph& graph);
void 	nb_bfs();




template <typename T>
std::function<bool (Tile &tile)> make_is_tile(T&& first)
{
	return first;
}

template <typename T, typename... Args>
std::function<bool (Tile &tile)> make_is_tile(T&& first, Args&&... args)
{
	return (	
		[&](Tile &tile)->bool 
		{
			return ( first(tile) && make_is_tile(std::forward<Args>(args)...)(tile) );
		} 
		);
}

template <typename T>
std::function<bool (Tile &tile)> is_not(T&& f)
{
	return std::not1((std::function<bool (Tile &tile)>)f);
}



#endif /* INCLUDES_HPP */
