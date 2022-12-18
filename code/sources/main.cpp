#include "includes.hpp"
#include "Tile.hpp"
#include "Data.hpp"


int turn;
int my_side;

int main()
{
    // while (1)
    // {
    //     std::string s;
    //     std::getline(std::cin, s);
    //     std::cerr << s << std::endl;
    // }
    
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

        // std::cerr << "tile 8 " << d.tiles[8].x << " " << d.tiles[8].y << std::endl;
        // std::cerr << "tile 8 " << d.getTile(0, 1)->x << " " << d.getTile(0, 1)->y << std::endl;
        // std::cerr << "tile 8 : id : " << d.getTile(0, 1)->id << std::endl;
        std::cerr << std:: endl;
        Graph g;
        for (auto& tile : d.tiles)
        {
            if (tile.scrap_amount && !tile.recycler)
            {
                g.addVertex(tile.id);
            }
            
        }
        for (auto& tile : d.tiles)
        {
            int x = tile.x;
            int y = tile.y;
            if (tile.scrap_amount && !tile.recycler)
            {
                for(auto& neighbor : { d.getUsableTile(x - 1, y),
                                        d.getUsableTile(x, y - 1),
                                        d.getUsableTile(x + 1, y),
                                        d.getUsableTile(x, y + 1),})
                {
                    
                    if (neighbor)
                    {
                        // std::cerr << "neighbor " << neighbor->x << " " << neighbor->y << std::endl;
                        // std::cerr << "neighbor id " << neighbor->id << std::endl;

                        g.addEdge(tile.id, neighbor->id);
                    }
                }
            }
            
        }
        bfs(g, 0);
        

        

	 	message("coucou");
        std::cout << std::endl;
        turn++;
    }
}
