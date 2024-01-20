#include <vector>
#include <string>
#include <cmath>

#include "Shotgun.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Data.h"
#include <iostream>
#include <string>
#include <sstream>

Shotgun::Shotgun(std::shared_ptr<Game_Object> const& init_player, Data const& game_data, Data_desc const& description)
: Weapon{init_player, game_data, description}, ammo{12}, uvRect{}
{
    weapon_time *= 2.f / 3.f;
    //Antimation
    sf::Texture const& texture {game_data.textures.at(description.texture_string)};
    uvRect.width = texture.getSize().x / 2;
    uvRect.height = texture.getSize().y / 2;
}

void Shotgun::update(sf::Time const& t_delta,
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
        offset.x = 2 * radius * cos(angle * M_PI / 180);
        offset.y = 2 * radius * sin(angle * M_PI / 180);
        offset += position_vec;
        
        for (int i {-3}; i <= 3; ++i)
        {
            game_objects.push_back(std::make_shared<Bullet>(offset.x, offset.y, game_data, Data_desc {"bullet", "bullet"}, angle + i * 5));
        }
        weapon_time = instance_stats.at("weapon_time");
        
        if (--ammo <= 0)
        {
            expired = true;
        }
    }
    //Animation
    Shotgun::animate();
}

std::string Shotgun::get_type_name() 
{
    return "shotgun";
}

void Shotgun::animate()
{
    sf::Vector2u current_image {};
    float time {instance_stats.at("weapon_time")};
    
    if (weapon_time > 2 * time / 3)
    {
        current_image.x = 1;
        current_image.y = 0;
    }
    else if (weapon_time > time / 3)
    {
        current_image.x = 0;
        current_image.y = 1;
    }
    else if (weapon_time > 0)
    {
        current_image.x = 1;
        current_image.y = 1;
    }
    else // Idle
    {
        current_image.x = 0;
        current_image.y = 0;
    }

    uvRect.left = current_image.x * uvRect.width;
    uvRect.top = current_image.y * uvRect.height;
    sprite.setTextureRect(uvRect);
}

std::string Shotgun::get_status()
{
    std::stringstream ss {};
    ss << "Ammo left: " << ammo;
    return ss.str();
}