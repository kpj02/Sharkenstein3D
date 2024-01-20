#include "Chainsaw.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Weapon.h"
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>

Chainsaw::Chainsaw(std::shared_ptr<Game_Object> const& init_player, Data const& game_data, Data_desc const& description)
: Weapon{init_player, game_data, description}, uvRect{}, blink{1}, chainsaw_sound{}
{
    //Antimation
    sf::Texture const& texture {game_data.textures.at(description.texture_string)};
    uvRect.width = texture.getSize().x;
    uvRect.height = texture.getSize().y / 2;
    chainsaw_sound.setBuffer(game_data.sounds.at("chainsaw_start_sound"));
    chainsaw_sound.play();
}

void Chainsaw::update(sf::Time const& t_delta,
                     Game_Object_Vector&)
{
    std::shared_ptr<Game_Object> player = player_ptr.lock();
    // Position
    sf::Vector2f offset {};
    offset.x = player->get_radius()*cos(player->get_angle() * M_PI / 180);
    offset.y = player->get_radius()*sin(player->get_angle() * M_PI / 180); 
    position_vec = player->get_position() + offset;

    angle = player->get_angle();

    // Duration
    if (weapon_time - t_delta.asSeconds() >= 0)
    {
        weapon_time -= t_delta.asSeconds();
    }
    else
    {
        expired = true;
    }
    //Animation
    Chainsaw::animate();
}

std::string Chainsaw::get_type_name() 
{
    return "chainsaw";
}

void Chainsaw::animate()
{
    sf::Vector2u current_image {};
    if (blink == 1)
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



/* Chainsaw is useless with map_collision but it is a requirement
void Chainsaw::map_collision(Map& map, Game_Object_Vector & object_vector)
{   
    if( map.has_collided(get_position().x, get_position().y, get_radius()) )
    {   
        expired = true;
    }
}
*/

std::string Chainsaw::get_status()
{
    std::stringstream ss {};
    ss << "Time left: ";
    if (weapon_time >10)
    {
        ss << static_cast<int>(weapon_time);
    }
    else if (weapon_time >1)
    {
        ss << "0" << static_cast<int>(weapon_time);
    }
    else
    {
        ss << "00";
    }
    ss << ":";
    
    if (weapon_time - static_cast<int>(weapon_time) > 0.01)
    {
        ss << static_cast<int>((weapon_time - static_cast<int>(weapon_time))*10);
    }
    else
    {
        ss << "0";
    }
    return ss.str();
}