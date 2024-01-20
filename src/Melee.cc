#include "Melee.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Projectile.h"
#include "Player.h"
#include "Gib.h"
#include "Chainpowerup.h"
#include <cmath>
#include "Map.h"
#include <iostream>
#include "Data.h"
#include <memory>

Melee::Melee(float const x, float const y, Data const& game_data, Data_desc const& description, 
             float const& theta, std::shared_ptr<Player> const& p_ptr, Map& map)
: Enemy{x, y, game_data, description, theta, p_ptr, map}
{
}

void Melee::update(sf::Time const& t_delta, Game_Object_Vector & object_vector)
{
    search(t_delta, object_vector);

    if(expired)
    {
        object_vector.push_back(std::make_shared<Gib>(position_vec.x, position_vec.y, game_data, Data_desc {"gib", "gib"}, 0));
    }
} 

//Moves the shark in the right direction
void Melee::get_next_movement(sf::Time const& t_delta, Game_Object_Vector&,
                              sf::Vector2f const distance_vec,
                              float const distance_vec_lenght, bool)
{
    position_vec += speed * t_delta.asSeconds() * distance_vec / distance_vec_lenght;
}

//Empty for the classes that dont override
void Melee::use_weapon(Game_Object_Vector&)
{}

std::string Melee::get_type_name() 
{
    return "melee";
}
