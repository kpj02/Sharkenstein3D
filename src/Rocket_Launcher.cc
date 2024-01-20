#include <vector>
#include <string>
#include <cmath>

#include "Rocket_Launcher.h"
#include "Weapon.h"
#include "Projectile.h"
#include "Data.h"
#include <iostream>

Rocket_Launcher::Rocket_Launcher(std::shared_ptr<Game_Object> const& init_player, Data const& game_data, Data_desc const& description)
: Weapon{init_player, game_data, description}, uvRect{}, switch_time {instance_stats.at("weapon_time")/3}
{
    //Antimation
    sf::Texture const& texture {game_data.textures.at(description.texture_string)};
    uvRect.width = texture.getSize().x / 3;
    uvRect.height = texture.getSize().y / 2;
    weapon_time = switch_time*2;
}

void Rocket_Launcher::update(sf::Time const& t_delta,
                     Game_Object_Vector& game_objects)
{
    std::shared_ptr<Game_Object> player = player_ptr.lock();
    // Update position
    sf::Vector2f offset {};
    offset.x = radius * cos(player->get_angle() * M_PI / 180);
    offset.y = radius * sin(player->get_angle() * M_PI / 180); 
    position_vec = player->get_position() + offset;

    // Angle
    angle = player->get_angle();

    // Fire away!
    if (weapon_time > 0)
    {
        weapon_time -= t_delta.asSeconds();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        sf::Vector2f projectile_offset {position_vec};
        projectile_offset.x += 2 * radius * cos(angle * M_PI / 180);
        projectile_offset.y += 2 * radius * sin(angle * M_PI / 180);
        game_objects.push_back(std::make_shared<Projectile>(projectile_offset.x, projectile_offset.y, game_data, Data_desc {"projectile", "projectile"}, angle));
        
        weapon_time = instance_stats.at("weapon_time");
    }

    //Animation
    Rocket_Launcher::animate();

}

std::string Rocket_Launcher::get_type_name() 
{
    return "rocket_launcher";
}

void Rocket_Launcher::animate()
{
    sf::Vector2u current_image {};
    current_image.y = 1;
    if (weapon_time >2*switch_time)
    {
        current_image.x = 0;
    }
    else if (weapon_time > switch_time)
    {
        current_image.x = 1;
    }
    else if (weapon_time > 0)
    {
        current_image.x = 2;
    }

    else
    {
        current_image.x = 1;
    }
    uvRect.left = current_image.x * uvRect.width;
    uvRect.top = current_image.y * uvRect.height;
    sprite.setTextureRect(uvRect);
}

std::string Rocket_Launcher::get_status()
{
    return "";
}