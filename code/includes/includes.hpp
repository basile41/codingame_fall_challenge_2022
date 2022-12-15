#ifndef INCLUDES_HPP
#define INCLUDES_HPP

# include <bits/types/wint_t.h>
# include <cstddef>
# include <iostream>
# include <string>
# include <vector>
# include <algorithm>

#define ME 1
#define ENEMY 0
#define NEUTRAL -1

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

void	message(std::string message);

#endif /* INCLUDES_HPP */
