#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include "Data.h"
#include "Gib.h"

Gib::Gib(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Static{x, y, game_data, description, theta}, time_alive{instance_stats.at("invincibility")}
{}   

void Gib::update(sf::Time const& t_delta, Game_Object_Vector&)
{
    time_alive -= t_delta.asSeconds();

    if(time_alive < 0)
    {
        expired = true;
    }
}

std::string Gib::get_type_name() 
{
    return "gib";
}

sf::Texture const* Gib::get_texture(float)
{
    if (time_alive > 0.35f)
    {
        return &game_data.textures.at("gib1");
    }
    else if (time_alive > 0.3f)
    {
        return &game_data.textures.at("gib2");
    }
     else if (time_alive > 0.25f)
    {
        return &game_data.textures.at("gib3");
    }
     else if (time_alive > 0.2f)
    {
        return &game_data.textures.at("gib4");
    }
     else if (time_alive > 0.15f)
    {
        return &game_data.textures.at("gib5");
    }
     else if (time_alive > 0.1f)
    {
        return &game_data.textures.at("gib6");
    }
     else if (time_alive > 0.05f)
    {
        return &game_data.textures.at("gib7");
    }
     else if (time_alive > 0.02f)
    {
        return &game_data.textures.at("gib8");
    }
    else
    {
        return &game_data.textures.at("gib9");
    }
}
