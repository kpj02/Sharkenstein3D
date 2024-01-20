#if !defined(MAP)
#define MAP

#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map_Tile.h"



class Map
{
public:
    Map(std::map<std::string,sf::Texture>&);

    void render_map(sf::RenderWindow&, sf::Vector2f const, float const);
    void render_map_2D(sf::RenderWindow&);
    void build_texture();
    bool has_collided(float, float, double);
    int drawend(float const);
    int drawstart(float const);
    float len(sf::Vector2f);
    float scalar_prod(sf::Vector2f, sf::Vector2f);
    void create_walls(sf::Vector2f const&, sf::Vector2f &, sf::Vector2f &);
    void create_floor_ceil_sprite(sf::Vector2f const&, sf::Vector2f &, sf::Vector2f &);
    void open_doors(int);
    std::vector< std::shared_ptr<Map_Tile> > map_tiles;
    int col_nr;
    int row_nr;
    std::vector<float> len_to_wall;

private:
    int fade_dist = 15;
    std::map<std::string,sf::Texture>& textures;
    std::vector<float> y_vector;

    // Variables for Floor and Ceiling render
    sf::Image floor_src_image;
    sf::Image ceil_src_image;
    sf::Image floor_ceil_image;
    sf::Texture floor_ceil_texture;
    sf::Sprite floor_ceil_sprite;

    // Variables for Wall render
    sf::VertexArray wall_lines;
    sf::VertexArray door_lines;
    sf::RenderTexture wall_render_texture;
    sf::Sprite wall_sprite;

    // Variabled for Minimap
    sf::RenderTexture texture_2D;
    sf::Sprite sprite_2D;
};

#endif // MAP
