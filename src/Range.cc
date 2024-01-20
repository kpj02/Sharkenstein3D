#include "Range.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Player.h"
#include "Map.h"
#include "Projectile.h"
#include <cmath>
#include <vector>
#include "Data.h"
#include "Gib.h"
#include <memory>

Range::Range(float const x, float const y, Data const& game_data, Data_desc const& description,
             float const& theta, std::shared_ptr<Player> const& p_ptr, Map& map)
: Enemy{x, y, game_data, description, theta, p_ptr, map}
{
}


void Range::update(sf::Time const& t_delta, Game_Object_Vector & object_vector)
{
    weapon_cooldown -= t_delta.asSeconds();
    search(t_delta, object_vector);
    if(expired)
    {
        object_vector.push_back(std::make_shared<Gib>(position_vec.x, position_vec.y, game_data, Data_desc {"gib", "gib"}, 0));
    }
}

//Moves the shark in the right direction
void Range::get_next_movement(sf::Time const& t_delta, Game_Object_Vector & object_vector,
                              sf::Vector2f const distance_vec, float const distance_vec_lenght, bool player_is_seen)
{
    if (distance_vec_lenght > 700 || !player_is_seen)
    {
        position_vec += speed * t_delta.asSeconds() * distance_vec / distance_vec_lenght;
    }
    set_angle(distance_vec);
    use_weapon(object_vector);
}

//Fires weapon in the right direction if its loaded
void Range::use_weapon(Game_Object_Vector & object_vector)
{
    if (weapon_cooldown > 0)
    {
        return;
    }
    weapon_cooldown = instance_stats.at("weapon_time");

    sf::Vector2f offset {};
    offset.x = 2 * radius * cos(angle * M_PI / 180);
    offset.y = 2 * radius * sin(angle * M_PI / 180);
    offset += position_vec;

    object_vector.push_back(std::make_shared<Projectile>(offset.x, offset.y, game_data, Data_desc {"projectile", "projectile"}, angle));
}

std::string Range::get_type_name() 
{
    return "range";
}
