#include "Minipowerup.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Data.h"
#include "Minigun.h"

Minipowerup::Minipowerup(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Power_Up{x, y, game_data, description, theta}
{}

std::string Minipowerup::get_type_name() 
{
    return "minipowerup";
}

std::shared_ptr<Weapon> Minipowerup::get_weapon(std::shared_ptr<Game_Object> const& player)
{
    return std::make_shared<Minigun>(player, game_data, Data_desc {"minigun", "minigun"});
}
