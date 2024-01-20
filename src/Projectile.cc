#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <memory>
#include "Melee.h"
#include "Explosion.h"
#include "Projectile.h"
#include "Player.h"
#include "Boss.h"
#include "Range.h"
#include "Data.h"

Projectile::Projectile(float const x, float const y, Data const& game_data, Data_desc const& description, float const& angle)
: Dynamic(x, y, game_data, description, angle)
{}

void Projectile::update(sf::Time const& t_delta, Game_Object_Vector&)
{
    position_vec.x += speed * cos(2 * M_PI * angle / 360) * t_delta.asSeconds();
    position_vec.y += speed * sin(2 * M_PI * angle / 360) * t_delta.asSeconds();
}

void Projectile::handle_collision(Game_Object* game_object, Game_Object_Vector& object_vector)
{
    if(dynamic_cast<Enemy*>(game_object) || dynamic_cast<Player*>(game_object))
    {
        expired = true;
        object_vector.push_back(std::make_shared<Explosion>(position_vec.x, position_vec.y, game_data, Data_desc {"explosion", "explosion"}, 0));
    }
}

void Projectile::map_collision(Map& map, Game_Object_Vector & object_vector)
{
    if( map.has_collided(get_position().x, get_position().y, get_radius() ) == true)
    {   
        expired = true;
        object_vector.push_back(std::make_shared<Explosion>(position_vec.x, position_vec.y, game_data, Data_desc {"explosion", "explosion"}, 0));
    }
}

std::string Projectile::get_type_name() 
{
    return "projectile";
}

sf::Texture const* Projectile::get_texture(float render_angle)
{
    if (-135.f - 22.5f <= render_angle && render_angle < -90.f - 22.5f)
    {
        return &game_data.textures.at("missile_side_front_right");
    }
    else if (-90.f - 22.5f <= render_angle && render_angle < -45.f - 22.5f)
    {
        return &game_data.textures.at("missile_side_right");
    }
    else if (-45.f - 22.5f <= render_angle && render_angle < - 22.5f)
    {
        return &game_data.textures.at("missile_side_back_right");
    }
    else if (-22.5f <= render_angle && render_angle < 22.5f) // Away from player
    {
        return &game_data.textures.at("missile_back");
    }
    else if (22.5f <= render_angle && render_angle < 45.f + 22.5f)
    {
        return &game_data.textures.at("missile_side_back_left");
    }
    else if (45.f + 22.5f <= render_angle && render_angle < 90.f + 22.5f)
    {
        return &game_data.textures.at("missile_side_left");
    }
    else if (90.f + 22.5f <= render_angle && render_angle < 135.f + 22.5f)
    {
        return &game_data.textures.at("missile_side_front_left");
    }
    else // Towards player
    {
        return &game_data.textures.at("missile_front");
    }
}
