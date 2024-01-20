#include "Star.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <cmath>

Star::Star(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Static{x, y, game_data, description, theta}
{}

void Star::handle_collision(Game_Object* game_object, Game_Object_Vector&)
{
    if (dynamic_cast<Player*>(game_object))
    {
        expired = true;
    }
}

void Star::update(sf::Time const& t_delta, Game_Object_Vector&)
{
    angle += instance_stats.at("angular_velocity")*t_delta.asSeconds();
    instance_stats.at("angular_velocity");
    wobble = 32*std::sin(angle/360.f * M_PI); 
}

std::string Star::get_type_name() 
{
    return "star";
}

sf::Texture const* Star::get_texture(float render_angle)
{
    if (-135.f - 22.5f <= render_angle && render_angle < -90.f - 22.5f)
    {
        return &game_data.textures.at("star_front_right");
    }
    else if (-90.f - 22.5f <= render_angle && render_angle < -45.f - 22.5f)
    {
        return &game_data.textures.at("star_right");
    }
    else if (-45.f - 22.5f <= render_angle && render_angle < - 22.5f)
    {
        return &game_data.textures.at("star_back_right");
    }
    else if (-22.5f <= render_angle && render_angle < 22.5f) // Away from player
    {
        return &game_data.textures.at("star_back");
    }
    else if (22.5f <= render_angle && render_angle < 45.f + 22.5f)
    {
        return &game_data.textures.at("star_back_left");
    }
    else if (45.f + 22.5f <= render_angle && render_angle < 90.f + 22.5f)
    {
        return &game_data.textures.at("star_left");
    }
    else if (90.f + 22.5f <= render_angle && render_angle < 135.f + 22.5f)
    {
        return &game_data.textures.at("star_front_left");
    }
    else // Towards player
    {
        return &game_data.textures.at("star_front");
    }
}
