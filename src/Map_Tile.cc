#include "Map_Tile.h"
#include <iostream>

Map_Tile::Map_Tile(int x, int y, char description, std::map<std::string,sf::Texture>& textures)
: x{x}, y{y}, description{description}, sprite{}
{   
    switch (description)
    {
    case 'W':
        sprite.setTexture(textures["ceiling"]);
        break;
    case 'V':
        break;

    case 'S':
        sprite.setTexture(textures["floor_with_star"]);
        break;
    default:
        if (isdigit(description))
        {
            sprite.setTexture(textures["minimap_door"]);
        }
        else
        {
            sprite.setTexture(textures["wood_floor"]);
        }
        break;
    }
    sprite.setTextureRect(sf::IntRect(0,0,64,64));
    sprite.setScale(4.f,4.f);
    sprite.setPosition(64 * 4 * x, 64 * 4 * y);
}