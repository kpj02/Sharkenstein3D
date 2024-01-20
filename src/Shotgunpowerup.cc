#include "Shotgunpowerup.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Data.h"
#include "Shotgun.h"

Shotgunpowerup::Shotgunpowerup(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Power_Up{x, y, game_data, description, theta}
{}

std::string Shotgunpowerup::get_type_name() 
{
    return "shotgunpowerup";
}

std::shared_ptr<Weapon> Shotgunpowerup::get_weapon(std::shared_ptr<Game_Object> const& player)
{
    return std::make_shared<Shotgun>(player, game_data, Data_desc {"shotgun", "shotgun"});
}
