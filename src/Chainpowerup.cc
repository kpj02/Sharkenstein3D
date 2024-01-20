#include "Chainpowerup.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Data.h"
#include "Chainsaw.h"

Chainpowerup::Chainpowerup(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Power_Up{x, y, game_data, description, theta}
{}

std::string Chainpowerup::get_type_name() 
{
    return "chainpowerup";
}

std::shared_ptr<Weapon> Chainpowerup::get_weapon(std::shared_ptr<Game_Object> const& player)
{
    return std::make_shared<Chainsaw>(player, game_data, Data_desc {"chainsaw", "chainsaw"});
}
