#include "Map.h"
#include "Map_Tile.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include "Data.h"

Map::Map(std::map<std::string,sf::Texture>& textures)
: map_tiles{}, col_nr{}, row_nr{0}, len_to_wall{}, textures{textures}, y_vector{},
  floor_src_image{}, ceil_src_image{}, floor_ceil_image{}, floor_ceil_texture{}, floor_ceil_sprite{},
  wall_lines{sf::Lines, 1280*2}, door_lines{sf::Lines, 1280*2}, wall_render_texture{}, wall_sprite{},
  texture_2D{}, sprite_2D{}
{
    std::fstream map_file {};
    std::string line {};
                      
    map_file.open("resources/data/map.txt", std::ios::in);
    
    // Make tile map from textfile and calculate number of rows and columns
    while(std::getline(map_file,line))
    {
        col_nr = 0;
        for (char& c : line)
        {   
            map_tiles.push_back(std::make_shared<Map_Tile>(col_nr, row_nr, c, textures));
            ++col_nr;
        } 
        ++row_nr;
    }
    map_file.close();

    floor_src_image.loadFromFile("resources/images/water_damaged_wood.png");
    ceil_src_image.loadFromFile("resources/images/blue_tile.png");
    floor_ceil_image.create(1280, 720);

    y_vector.reserve(screen_width);

    wall_render_texture.create(1280, 720);
}

void Map::build_texture()
{   
    texture_2D.create(col_nr * 256, row_nr * 256);
    for (auto const& map_tile : map_tiles)
    {   
        texture_2D.draw(map_tile->sprite);
    }
    texture_2D.display();
    sprite_2D.setTexture(texture_2D.getTexture());
}

void Map::render_map_2D(sf::RenderWindow& window)
{
    window.draw(sprite_2D);
}

bool Map::has_collided(float x, float y, double radius)
{   
    int player_row { static_cast<int>(std::floor(x / (64*4)))};
    int player_col { static_cast<int>(std::floor(y / (64*4)))};

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if( i + player_row >=0 && j + player_col >= 0 && j + player_col < row_nr && i + player_row < col_nr)
            {
                int new_row { static_cast<int>(floor( (x + i * radius) / (64 * 4))) };
                int new_col { static_cast<int>(floor( (y + j * radius) / (64 * 4))) };

                if ( new_row == i + player_row && new_col == j + player_col && map_tiles.at(new_row + col_nr * new_col)->description != ' ')
                {
                    return true;
                }
            }
        }
    }
    return false;
}

/*
We have used inspiration from 
Lodev: https://lodev.org/cgtutor/raycasting.html 
Permadi: https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/
Liam Wynn: https://wynnliam.github.io/raycaster/news/tutorial/2019/04/03/raycaster-part-02.html
for the ray casting.
*/

void Map::render_map(sf::RenderWindow& window,
                     sf::Vector2f const player_pos_vec,
                     float const player_rot)
{
    
    
    // Vektor som pekar i den riktning som spelaren tittar
    sf::Vector2f direction_vec {static_cast<float>(std::cos(player_rot * M_PI / 180)),
                                static_cast<float>(std::sin(player_rot * M_PI / 180))};
    
    // Vektor som är ortagonal mot riktnings vektorn, ger kamera planet.
    sf::Vector2f plane_vec {static_cast<float>(-std::sin(player_rot * M_PI / 180)),
                            static_cast<float>(std::cos(player_rot * M_PI / 180))};

    create_walls(player_pos_vec, direction_vec, plane_vec);

    create_floor_ceil_sprite(player_pos_vec, direction_vec, plane_vec);
    
    //Rendera ut golv + tak som en sprite med framtagna texturen.
    floor_ceil_texture.loadFromImage(floor_ceil_image);
    floor_ceil_sprite.setTexture(floor_ceil_texture);
    window.draw(floor_ceil_sprite);

    wall_render_texture.clear(sf::Color(0,0,0,0));

    sf::RenderStates states;
    states.texture = &textures["wall"];
    wall_render_texture.draw(wall_lines, states);
    states.texture = &textures["door"];
    
    wall_render_texture.draw(door_lines, states);
    wall_render_texture.display();
    wall_sprite.setTexture(wall_render_texture.getTexture());
    
    window.draw(wall_sprite);
    return;
}

int Map::drawstart(float const distance_wall)
{
    float line_height {static_cast<float>((screen_width / 2) / distance_wall)};
    float draw_midpoint {static_cast<float>(0.5 * screen_height)};
    float draw_start {draw_midpoint - line_height / 2};

    return draw_start;
}

int Map::drawend(float const distance_wall)
{
    float line_height {static_cast<float>((screen_width / 2) / distance_wall)};
    float draw_midpoint {static_cast<float>(0.5 * screen_height)};
    float draw_end {draw_midpoint + line_height / 2};

    return draw_end;
}

float Map::len(sf::Vector2f const vect)
{
    return std::sqrt(vect.x*vect.x + vect.y * vect.y);
}

float Map::scalar_prod(sf::Vector2f const vect1, sf::Vector2f const vect2)
{
    return vect1.x*vect2.x + vect1.y*vect2.y; 
}

void Map::create_walls(sf::Vector2f const& player_pos_vec, sf::Vector2f &direction_vec, sf::Vector2f &plane_vec)
{   
    y_vector.clear();
    wall_lines.clear();
    door_lines.clear();
    len_to_wall.clear(); 

    //Beräkar rutan spelaren befinner sig i
    int player_x { static_cast<int>(std::floor(player_pos_vec.x / (64*4)))};
    int player_y { static_cast<int>(std::floor(player_pos_vec.y / (64*4)))};

    // Here we used code and inspiration from Lodev
    // wall casting
    for (int x {0}; x <= screen_width; x+=1)
    {   
        float camera_x {static_cast<float>(x*2.0 / screen_width - 1)};
        sf::Vector2f ray_dir {direction_vec + camera_x * plane_vec };

        int map_x {player_x};
        int map_y {player_y};

        float delta_x {std::abs(1/ray_dir.x)};
        float delta_y {std::abs(1/ray_dir.y)};

        int step_x {};
        int step_y {};

        float side_dist_x {};
        float side_dist_y {};

        if (ray_dir.x < 0)
        {
            step_x = -1;
            side_dist_x = (player_pos_vec.x/256 - map_x) * delta_x;
        }
        else
        {
            step_x = 1;
            side_dist_x = (-player_pos_vec.x/256 + map_x + 1.0) * delta_x;

        }
        if (ray_dir.y < 0)
        {
            step_y = -1;
            side_dist_y = (player_pos_vec.y/256 - map_y) * delta_y;
        }
        else
        {
            step_y = 1;
            side_dist_y = (-player_pos_vec.y/256 + map_y + 1.0) * delta_y;
        }

        float distance_wall {};

        int side {};
        std::string tile_texture{};
        while(1)
        {
            if(side_dist_x < side_dist_y)
            {
                side_dist_x += delta_x;
                map_x += step_x;
                side = 0;
            }
            else
            {
                side_dist_y += delta_y;
                map_y += step_y;
                side = 1;
            }
            if (map_tiles.at(map_x + col_nr * map_y)->description == 'W')
            {
                tile_texture = "wall";
                break;
            }
            else if (map_tiles.at(map_x + col_nr * map_y)->description != ' ')
            {
                tile_texture = "door";
                break;
            }
        }

        int draw_start{};
        int draw_end{};
        sf::Color color {0,0,0};

        if (side == 0)
        {
            distance_wall = (side_dist_x-delta_x);
            if (distance_wall < fade_dist)
            {
                color.r = 255 * (1 - distance_wall / fade_dist);
                color.g = 255 * (1 - distance_wall / fade_dist);
                color.b = 255 * (1 - distance_wall / fade_dist);
            }          
        }
        else
        {
            distance_wall = (side_dist_y-delta_y);
            if (distance_wall < fade_dist)
            {
                color.r = 200 * (1 - distance_wall / fade_dist);
                color.g = 200 * (1 - distance_wall / fade_dist);
                color.b = 200 * (1 - distance_wall / fade_dist);
            }
        }
        
        draw_start = drawstart(distance_wall);
        draw_end = drawend(distance_wall);
        
        len_to_wall.push_back(distance_wall);

        y_vector.push_back(draw_end - 1);

        // Hitta den relativa textur kordinaten
        float wall_x{};
        // krockar med x-vägg
        if (side == 0)
        {
            wall_x = player_pos_vec.y/256 + distance_wall * ray_dir.y; 
        }
        // krockar med y-vägg
        else
        {
            wall_x = player_pos_vec.x/256 + distance_wall * ray_dir.x;
        }

        wall_x -= std::floor(wall_x);

        int tex_x {static_cast<int>(wall_x*64)};

        if ((side == 0 && ray_dir.x > 0) || (side == 1 && ray_dir.y < 0))
        {
            tex_x = 64 - tex_x - 1;            
        }

        if (tile_texture == "door") // Dörrar
        {
            door_lines.append(sf::Vertex(sf::Vector2f(x, draw_start),
                        color,sf::Vector2f(tex_x,0.f)));
            door_lines.append(sf::Vertex(sf::Vector2f(x, draw_end),
                        color,sf::Vector2f(tex_x,64.f)));
        }
        else // Väggar
        {
            wall_lines.append(sf::Vertex(sf::Vector2f(x, draw_start),
                        color,sf::Vector2f(tex_x,0.f)));
            wall_lines.append(sf::Vertex(sf::Vector2f(x, draw_end),
                        color,sf::Vector2f(tex_x,64.f)));
        }
    }
    return;
}

void Map::create_floor_ceil_sprite(sf::Vector2f const& player_pos_vec, sf::Vector2f &direction_vec, sf::Vector2f &plane_vec)
{
    sf::Vector2f ray_dir_0 {direction_vec - plane_vec };
    sf::Vector2f ray_dir_1 {direction_vec + plane_vec };
    float pos_z = screen_width/4;
    for (int y {screen_height/2}; y < screen_height; y += 2)
    {
        // vektorer som spänner upp fov-konen
        
        int p = y - screen_height / 2;
        
        float row_distance = pos_z/p;

        // Vektor som itereras med för att stega upp en pixel
        sf::Vector2f floor_step {row_distance/screen_width * (ray_dir_1-ray_dir_0)};
        
        // Vektor som pekar på första positionen.
        sf::Vector2f floor_vec {player_pos_vec + 256 * row_distance * ray_dir_0
                                - 2.f * 256.f * floor_step};

        for(int x{0}; x < screen_width; x += 2)
        {
            floor_vec = floor_vec + 2.f * 256.f * floor_step;
            
            // Only render floor and ceiling within view-distance (fade_dist)
            // and infront of walls
            if (row_distance > fade_dist || y < y_vector.at(x))
            {
                continue;
            }
            
            // Texture coordinates to get pixel color for ceil and floor render
            sf::Vector2i tex_cord_floor {(64*(static_cast<int>(floor_vec.x) % 256)) / 256,
                                         (64*(static_cast<int>(floor_vec.y) % 256)) / 256};
            // Make sure tex_cord_floor is in range [0,64) (z % 256 can be in range (-256,256))
            if (tex_cord_floor.x < 0)
            {
                tex_cord_floor.x += 64;
            }
            if (tex_cord_floor.y < 0)
            {
                tex_cord_floor.y += 64;
            }

            // Create texture for floor
            sf::Color pixel_color {floor_src_image.getPixel(tex_cord_floor.x, tex_cord_floor.y)};

            pixel_color.r = pixel_color.r * (1 - row_distance / (fade_dist * 1));
            pixel_color.g = pixel_color.g * (1 - row_distance / (fade_dist * 1));
            pixel_color.b = pixel_color.b * (1 - row_distance / (fade_dist * 1));

            floor_ceil_image.setPixel(x    , y    , pixel_color);
            floor_ceil_image.setPixel(x    , y + 1, pixel_color);
            floor_ceil_image.setPixel(x + 1, y    , pixel_color);
            floor_ceil_image.setPixel(x + 1, y + 1, pixel_color);

            // Create texture for ceiling
            pixel_color = ceil_src_image.getPixel(tex_cord_floor.x, tex_cord_floor.y);

            pixel_color.r = pixel_color.r * (1 - row_distance / (fade_dist * 1));
            pixel_color.g = pixel_color.g * (1 - row_distance / (fade_dist * 1));
            pixel_color.b = pixel_color.b * (1 - row_distance / (fade_dist * 1));

            floor_ceil_image.setPixel(x    , screen_height - y - 1, pixel_color);
            floor_ceil_image.setPixel(x    , screen_height - y - 2, pixel_color);
            floor_ceil_image.setPixel(x + 1, screen_height - y - 1, pixel_color);
            floor_ceil_image.setPixel(x + 1, screen_height - y - 2, pixel_color);
        }
    }
}

void Map::open_doors(int const door_number)
{
    for (auto& map_tile : map_tiles)
    {
        // If map_tile has the same door number replace door tile with floor
        if (map_tile->description == '0' + door_number)
        {
            map_tile = std::make_shared<Map_Tile>
            (
                map_tile->x, map_tile->y, ' ', textures
            );
        }
    }
}
