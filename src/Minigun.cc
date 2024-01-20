#include <vector>
#include <string>
#include <cmath>

#include "Minigun.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Data.h"
#include <iostream>
#include <string>
#include <sstream>

Minigun::Minigun(std::shared_ptr<Game_Object> const& init_player, Data const& game_data, Data_desc const& description)
: Weapon{init_player, game_data, description}, ammo{200}, uvRect{}, blink{1}
{
    //Antimation
    sf::Texture const& texture {game_data.textures.at(description.texture_string)};
    uvRect.width = texture.getSize().x;
    uvRect.height = texture.getSize().y / 2;
}

void Minigun::update(sf::Time const& t_delta,
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
        sf::Vector2f bullet_offset {position_vec};
        bullet_offset.x += 2 * radius * cos(angle * M_PI / 180);
        bullet_offset.y += 2 * radius * sin(angle * M_PI / 180);
        game_objects.push_back(std::make_shared<Bullet>(bullet_offset.x, bullet_offset.y, game_data, Data_desc {"bullet", "bullet"}, angle));
        
        weapon_time = instance_stats.at("weapon_time");
        
        if (--ammo <= 0)
        {
            expired = true;
        }
    }
    //Animation
    Minigun::animate();
}

std::string Minigun::get_type_name() 
{
    return "Minigun";
}

//Changes texture when shooting
void Minigun::animate()
{
    sf::Vector2u current_image {};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && blink == 1)
    {
        current_image.y = 1;
        blink = 0;
    }
    else
    {
        current_image.y = 0;
        blink = 1;
    }
    uvRect.left = 0;
    uvRect.top = current_image.y * uvRect.height;
    sprite.setTextureRect(uvRect);
}

std::string Minigun::get_status()
{
    std::stringstream ss {};
    ss << "Ammo left: " << ammo;
    return ss.str();
}