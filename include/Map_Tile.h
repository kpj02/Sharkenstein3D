#if !defined(MAP_TILE)
#define MAP_TILE

#include <SFML/Graphics.hpp>
#include <vector>


class Map_Tile
{
public:
    Map_Tile(int, int, char, std::map<std::string,sf::Texture>&);

    int x;
    int y;
    char description;

    sf::Sprite sprite;
private:
};

#endif // MAP
