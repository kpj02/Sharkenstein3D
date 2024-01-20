#include "Game_Object.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cmath>
#include "Data.h"
#include <iostream>
#include <memory>

Game_Object::Game_Object(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: data_desc{description}, game_data{game_data}, instance_stats{game_data.stats.at(data_desc.data_string)},
  sprite{}, sprite_render_texture{}, position_vec{sf::Vector2f(x,y)}, angle{theta},
  radius{instance_stats.at("radius")}, wobble{0}, expired{false}
{
    sprite_render_texture.create(1280,720);
}

void Game_Object::set_up_sprite(sf::Texture&, float)
{   
    return;
}

//Empty for the classes that dont override
void Game_Object::handle_collision(Game_Object*, Game_Object_Vector&)
{}

/*
We have used inspiration from 
Liam Wynn: https://wynnliam.github.io/raycaster/news/tutorial/2019/04/03/raycaster-part-02.html
for "sprite" rendering.
*/

void Game_Object::render(sf::RenderWindow & window, sf::Vector2f based_vect, float player_angle, std::vector<float> len_to_wall)
{   
    sf::VertexArray sprite_lines{sf::Lines};
    
    float straight_len {static_cast<float>(1/std::sqrt(2)*(based_vect.x + based_vect.y))};
    float perpendicular_len {static_cast<float>(1/std::sqrt(2)*(-based_vect.x + based_vect.y))};
    
    float sprite_height {(1280 / 2) * instance_stats.at("sprite_height") / straight_len};
    float half_sprite_width {(1280 / 4) * instance_stats.at("sprite_width") / straight_len};
    float float_point {2 * (instance_stats.at("float_point") + wobble )/ straight_len * 256};

    float line_height {static_cast<float>((1280 / 2) / straight_len * 256)};
    float middle_screen_y {static_cast<float>(0.5 * 720)};
    float line_bottom {middle_screen_y + line_height / 2 - float_point};
    
    float render_angle {static_cast<float>(std::fmod(player_angle-angle, 360))};
    if (render_angle > 180)
    {
        render_angle -= 360;
    }
    else if (render_angle < -180)
    {
        render_angle += 360;
    }

    sf::Texture const* object_texture {get_texture(render_angle)};
    sf::Vector2u texture_size = object_texture->getSize();

    bool can_be_seen {false};

    // Create lines with the correct texture of the "sprite".
    for (int sprite_col = -half_sprite_width; sprite_col < half_sprite_width; sprite_col++)
    {   
        float middle_sprite_x {640 + perpendicular_len/straight_len * 640};

        // Only create line if in front of wall.
        if (len_to_wall.at(std::clamp(static_cast<int>(middle_sprite_x + sprite_col), 0, 1279)) > straight_len / 256 && straight_len / 256 < 15)
        {   
            can_be_seen = true;

            sf::Color color {255,255,255};
            color.r *= (1 - straight_len / (15 * 256));
            color.g *= (1 - straight_len / (15 * 256));
            color.b *= (1 - straight_len / (15 * 256));

            float texture_x = (sprite_col + half_sprite_width) / (2 * half_sprite_width) * texture_size.x;
            sprite_lines.append(sf::Vertex(sf::Vector2f(middle_sprite_x + sprite_col, line_bottom - sprite_height), color, sf::Vector2f(texture_x, 0)));
            sprite_lines.append(sf::Vertex(sf::Vector2f(middle_sprite_x + sprite_col, line_bottom), color, sf::Vector2f(texture_x, texture_size.y)));
        }
    }
    
    // Only render the "sprite" if at least one line can be seen.
    if (can_be_seen == true)
    {
        sf::RenderStates states;
        
        states.texture = object_texture;

        sprite_render_texture.clear(sf::Color(0,0,0,0));

        sprite_render_texture.draw(sprite_lines, states);
        sprite_render_texture.display();

        sprite.setTexture(sprite_render_texture.getTexture());
        
        window.draw(sprite);
    }
}

sf::Texture const* Game_Object::get_texture(float)
{
    return &game_data.textures.at(data_desc.texture_string);
}

sf::Vector2f Game_Object::get_position()
{
    return position_vec;
}

float Game_Object::get_angle()
{
    return angle;
}

float Game_Object::get_radius()
{
    return radius;
}

bool Game_Object::has_collided(Game_Object* const game_object) const
{
    return (std::pow(game_object->get_position().x - position_vec.x, 2) + 
            std::pow(game_object->get_position().y - position_vec.y, 2)) <= 
            std::pow(radius + game_object->get_radius(), 2);
}

bool Game_Object::is_expired() const
{
    return expired;
}

void Game_Object::map_collision(Map&, Game_Object_Vector&)
{
    return;
}

std::string Game_Object::get_data_string()
{
    return data_desc.data_string;
}

