#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"


int turn;
int my_side;

int main()
{
    turn = 0;
    int width;
    int height;
    std::cin >> width >> height; std::cin.ignore();
	// Data d(width, height);

    // game loop
    while (42) 
	{
        Data d;
        d.width = width;
        d.height = height;
		d.read();

        if (turn == 0)
                my_side = d.my_tiles[0]->x < width / 2 ? LEFT : RIGHT;

        auto first_unit = *std::max_element(d.my_units.begin(), d.my_units.end(), 
                            [](const Tile* t1, const Tile* t2)
                            { return (t1->x < t2->x) ^ my_side; });
        first_unit->spawn(1);
        for (auto current : d.my_units)
        {
            current->move(current->units, 10, 4);
        }
        // d.my_units[1]->move(1, 5, 5);
	 	message("coucou");
        std::cout << std::endl;
        turn++;
    }
}
