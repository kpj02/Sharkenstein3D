#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <memory>
#include "Melee.h"
#include "Explosion.h"
#include "Bullet.h"
#include "Player.h"
#include "Boss.h"
#include "Range.h"
#include "Data.h"


Bullet::Bullet(float const x, float const y, Data const& game_data, Data_desc const& description, float const& angle)
: Dynamic(x, y, game_data, description, angle)
{}

void Bullet::update(sf::Time const& t_delta, std::vector<std::shared_ptr<Game_Object>>&)
{
    position_vec.x += speed * cos(2 * M_PI * angle / 360) * t_delta.asSeconds();
    position_vec.y += speed * sin(2 * M_PI * angle / 360) * t_delta.asSeconds();
}

void Bullet::handle_collision(Game_Object* game_object, std::vector<std::shared_ptr<Game_Object>> & object_vector)
{
    if(dynamic_cast<Enemy*>(game_object))
    {
        expired = true;
        object_vector.push_back(std::make_shared<Explosion>(position_vec.x, position_vec.y, game_data, Data_desc {"explosion", "bullet_explosion"}, 0));
    }
}

void Bullet::map_collision(Map& map, std::vector<std::shared_ptr<Game_Object>> & object_vector)
{   
    if( map.has_collided(get_position().x, get_position().y, get_radius() ) == true)
    {   
        expired = true;
        object_vector.push_back(std::make_shared<Explosion>(position_vec.x, position_vec.y, game_data, Data_desc {"explosion", "bullet_explosion"}, 0));
    }
}

std::string Bullet::get_type_name() 
{
    return "bullet";
}
