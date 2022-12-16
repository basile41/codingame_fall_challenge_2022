#ifndef INCLUDES_HPP
#define INCLUDES_HPP

# include <bits/types/wint_t.h>
# include <cstddef>
# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <algorithm>

# include "Tile.hpp"
# include "Data.hpp"

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

#endif /* INCLUDES_HPP */
