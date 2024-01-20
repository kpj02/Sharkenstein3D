#include "Explosion.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include "Data.h"

Explosion::Explosion(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Static{x, y, game_data, description, theta}, time_alive{instance_stats.at("invincibility")}, explosion_sound{}
{
    explosion_sound.setBuffer(game_data.sounds.at("explosion"));
    explosion_sound.play();
}

void Explosion::update(sf::Time const& t_delta, Game_Object_Vector&)
{
    time_alive -= t_delta.asSeconds();

    if(time_alive < 0)
    {
        expired = true;
    }
}

std::string Explosion::get_type_name() 
{
    return "explosion";
}