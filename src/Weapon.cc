#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Weapon.h"
#include "Game_Object.h"
#include "Data.h"

Weapon::Weapon(std::shared_ptr<Game_Object> const& init_player, Data const& game_data, Data_desc const& description)
: Dynamic{init_player->get_position().x, init_player->get_position().y,
  game_data, description, init_player->get_angle()}, player_ptr{init_player},
  weapon_time{instance_stats.at("weapon_time")}, font {}, text {}
{
    load_text();
    std::shared_ptr<Game_Object> player = player_ptr.lock();
    if (!player)
    {
        throw std::runtime_error("Player pointer is expired in Weapon constructor");
    }
    sprite.setTexture(game_data.textures.at(data_desc.texture_string));
}

void Weapon::set_expired()
{
    expired = true;
}

void Weapon::render(sf::RenderWindow &, sf::Vector2f, float, std::vector<float>)
{}

void Weapon::render_hud(sf::RenderWindow& window)
{
    //Get size and position for the sprites
    float scale_width {game_data.stats.at(data_desc.data_string).at("sprite_width")};
    float scale_height {game_data.stats.at(data_desc.data_string).at("sprite_height")};
    float pos_offset_x {game_data.stats.at(data_desc.data_string).at("sprite_offset_x")};
    float pos_offset_y {game_data.stats.at(data_desc.data_string).at("sprite_offset_y")};

    //Set scale
    sf::Vector2u window_size {1280, 720};
    sf::Vector2f texture_size {sprite.getTexture()->getSize()};
    sprite.setScale(window_size.x * scale_width / texture_size.x, window_size.y * scale_height / texture_size.y);

    //Set position
    sf::Vector2f sprite_size = sprite.getScale();
    sprite.setOrigin(sprite_size.x/ 2, sprite_size.y / 2);
    sprite.setPosition(window_size.x*pos_offset_x, window_size.y*pos_offset_y);
    sprite.setRotation(0);
    window.draw(sprite);

    //Set text for the weapon
    text.setString(get_status());
    text.setCharacterSize(window_size.y / 6);
    unsigned int width = window_size.x*0.5;
    unsigned int height = window_size.y*0.77;
    text.setPosition(width, height);
    window.draw(text);
    
    
}

void Weapon::load_text()
{
    if (!font.loadFromFile("resources/fonts/GresickMetal-51LXV.otf"))
    {
        std::cout << "Error loading font" << std::endl;
        return;
    } 
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    }