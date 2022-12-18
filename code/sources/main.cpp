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


        debug(&d.tiles[1], d.tiles[1]);
        debug("get", d.getValidTile(1, 0));
        debug("get", d.getValidTile(1, 0));
        debug(d.my_tiles[0], *d.my_tiles[0]);
        debug(d.my_units[0], *d.my_units[0]);
        std::cerr << "---------------------------" << std::endl;

        for (auto& tile : d.tiles)
        {
            debug(&tile, tile);
        }
        std::cerr << "---------------------------" << std::endl;

        auto t = setTerritory(d.my_tiles[0]);
        debug("size :", t.size());
        for (auto& tile : t)
        {
            debug(tile, *tile);
        }

        if (turn == 0)
                my_side = d.my_tiles[0]->x < width / 2 ? LEFT : RIGHT;

        
        auto me_it = std::max_element(d.my_units.begin(), d.my_units.end(), 
                            [](const Tile* t1, const Tile* t2)
                            { return (t1->x < t2->x) ^ my_side; });
        auto opp_it = std::max_element(d.opp_units.begin(), d.opp_units.end(), 
                            [](const Tile* t1, const Tile* t2)
                            { return (t1->x > t2->x) ^ my_side; });
        auto me_unit = *me_it;
        auto opp_unit = *opp_it;
        me_unit->spawn(1);
        me_unit->move(me_unit->units, *opp_unit);

        d.my_units.erase(me_it);
        

        for (auto& current : d.my_units)
        {
                // current->debug();
            // current->move(current->units, *current->left);
            // current->move(current->units, *d.opp_tiles[0]);
            if (my_side == RIGHT)
            {
                if (current->left)
                    current->move(current->units, *current->left);
                else if (current->top)
                    current->move(current->units, *current->top);
                else if (current->bottom)
                    current->move(current->units, *current->bottom);
                else
                    current->move(current->units, *opp_unit);
            }
            else
            {
                if (current->right)
                    current->move(current->units, *current->right);
                else if (current->top)
                    current->move(current->units, *current->top);
                else if (current->bottom)
                    current->move(current->units, *current->bottom);
                else
                    current->move(current->units, *opp_unit);

            }

        }
        // d.my_units[1]->move(1, 5, 5);
	 	message("coucou");
        std::cout << std::endl;
        turn++;
    }
}
