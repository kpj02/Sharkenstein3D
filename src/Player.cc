#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <memory>
#include "Map.h"
#include "Melee.h"
#include "Star.h"
#include "Player.h"
#include "Boss.h"
#include "Projectile.h"
#include "Bullet.h"
#include "Rocket_Launcher.h"
#include "Power_Up.h"
#include "Chainpowerup.h"
#include "Chainsaw.h"
#include "Data.h"
#include "Minigun.h"
#include "Minipowerup.h"

Player::Player(float const x, float const y, Data const& game_data,
               Data_desc const& data_desc, float const theta, Map& map)
: Mortal{x, y, game_data, data_desc, theta}, stars_left{},
  angular_velocity{instance_stats.at("angular_velocity")}, weapon{},
  next_movement{}, won_game{false}, map{map}, font {}, damage_sound {}
{
    if (!font.loadFromFile("resources/fonts/GresickMetal-51LXV.otf"))
    {
        std::cout << "Error loading font" << std::endl;
        return;
    }

    sprite.setTexture(game_data.textures.at(data_desc.texture_string));
    sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x / 2,
                                  sprite.getTexture()->getSize().y / 2));
    sprite.setScale(sf::Vector2f(instance_stats.at("radius") 
                                 / sprite.getTexture()->getSize().x * 3,
                                 instance_stats.at("radius") 
                                 / sprite.getTexture()->getSize().y * 3));
}

void Player::init_weapon(Game_Object_Vector& game_objects)
{
    std::shared_ptr<Game_Object> player_shared = shared_from_this();
    
    if (!player_shared)
    {
        throw std::runtime_error("Attempted to use a null shared_ptr.");
    }
    
    Data_desc rocket_launcher_desc {"rocket_launcher", "rocket_launcher"};
    weapon = std::make_shared<Rocket_Launcher>
    (
        player_shared, game_data, rocket_launcher_desc
    );
    game_objects.push_back(weapon);
}

void Player::set_stars_left(Data const& game_data)
{
    stars_left = game_data.stars;
}

int Player::get_stars_left()
{
    return stars_left;
}

void Player::handle_collision(Game_Object* game_object,
                              Game_Object_Vector & object_vector)
{
    if((  dynamic_cast<Enemy*>(game_object)
       || dynamic_cast<Projectile*>(game_object)) && invincibility <= 0)
    {
        damage_sound.setBuffer(game_data.sounds.at("damage"));
        damage_sound.play();
        
        hp -= 1;
        invincibility = instance_stats.at("invincibility");
    } 
    if (dynamic_cast<Star*>(game_object))
    {
        if (stars_left == 1)  // Last star spawn boss
        {
            Data_desc boss_desc {"boss", "boss"};
            object_vector.push_back
            (
                std::make_shared<Boss>
                (
                    game_data, boss_desc, 0, shared_from_this(), map
                )
            );
        }
        if (stars_left > 0)
        {
            --stars_left;
        }
        else if (stars_left == 0)  // Grand star is collected and game is won
        {
            won_game = true;
        }
    }
    if (Power_Up* power_up = dynamic_cast<Power_Up*>(game_object))
    {
        weapon->set_expired();
        weapon = power_up->get_weapon(shared_from_this());
        object_vector.push_back(weapon);
    }
}

void Player::update(sf::Time const& t_delta, Game_Object_Vector & object_vector)
{
    // Rotation
    float rotation_multiplier {1};
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
    {
        //För att kunna rotera långsammare
        rotation_multiplier = 0.5f;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        angle -= angular_velocity * rotation_multiplier * t_delta.asSeconds();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        angle += angular_velocity * rotation_multiplier * t_delta.asSeconds();
    }

    sprite.setRotation(angle);

    // Movement
    get_next_movement(t_delta);

    // Invincibillity
    if (invincibility > 0)
    {
        invincibility -= t_delta.asSeconds();
    }

    // Return to default weapon if current is expired
    if (weapon->is_expired())
    {
        Data_desc rocket_launcher_desc {"rocket_launcher", "rocket_launcher"};
        weapon = std::make_shared<Rocket_Launcher>
        (
            shared_from_this(), game_data, rocket_launcher_desc
        );
        object_vector.push_back(weapon);
    }
}

void Player::set_position(float const x, float const y)
{
    position_vec.x = x;
    position_vec.y = y;
}

void Player::map_collision(Map& map,Game_Object_Vector&)
{   
    sf::Vector2f next_position {position_vec + next_movement};
    // Next position is not in a wall
    if(!map.has_collided(next_position.x, next_position.y, get_radius()))
    {
        position_vec += next_movement;
    }
    // Next x position is not in a wall
    else if(!map.has_collided(next_position.x, position_vec.y, get_radius()))
    {   
        position_vec.x += next_movement.x;
    }
    // Next y position is not in a wall
    else if(!map.has_collided(position_vec.x, next_position.y, get_radius()))
    {   
        position_vec.y += next_movement.y;
    }
    sprite.setPosition(position_vec);
}

// Calculates players next movement based on keyboard input
void Player::get_next_movement(sf::Time const& t_delta)
{
    next_movement = sf::Vector2f(0, 0);

    // Left
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        next_movement.x += cos(2 * M_PI * angle / 360 - M_PI_2);
        next_movement.y += sin(2 * M_PI * angle / 360 - M_PI_2);
    }
    // Right
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        next_movement.x += cos(2 * M_PI * angle / 360 + M_PI_2);
        next_movement.y += sin(2 * M_PI * angle / 360 + M_PI_2);
    }
    // Backwards
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        next_movement.x += cos(2 * M_PI * angle / 360  + M_PI);
        next_movement.y += sin(2 * M_PI * angle / 360  + M_PI);
    }
    // Forward
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        next_movement.x += cos(2 * M_PI * angle / 360);
        next_movement.y += sin(2 * M_PI * angle / 360);
    }
    
    float next_movement_norm
    {
        static_cast<float>(sqrt(  pow(next_movement.x, 2)
                                + pow(next_movement.y, 2)))
    };
    
    // Check if a direction is well defined (norm can be 1 or sqrt(2) so .1 is enough)
    if (next_movement_norm > .1f)
    {
        next_movement *= 1 / next_movement_norm;
    }
    next_movement *= speed * t_delta.asSeconds();
    return;
}

bool Player::has_won() const
{
    return won_game;
}

bool Player::is_dead() const
{
    return hp <= 0;
}

void Player::render_2d(sf::RenderWindow& window)
{
    window.draw(sprite);
    return;
}

void Player::render_hud(sf::RenderWindow& window, int score, int time)
{
    // Render the weapon sprite 
    weapon->render_hud(window);

    // Give screen red hue when player takes damage
    if(invincibility > 0)
    {
        float intensity {invincibility / instance_stats.at("invincibility")};
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(screen_width, screen_height));
        rectangle.setPosition(0, 0);
        rectangle.setFillColor(sf::Color{255,0,0, sf::Uint8(intensity*255)});
        window.draw(rectangle);
    }

    // Render the text part of the HUD
    sf::Text text {};
    std::stringstream ss {};
    
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setCharacterSize(screen_height / 6);
    
    // HP in bottom left corner
    ss << "HP: " << get_hp(); 
    text.setString(ss.str());
    text.setPosition(screen_width*0.05,
                     screen_height*0.95 - 1.5*text.getLocalBounds().height);
    window.draw(text);
    
    // Score in top left corner
    ss.str("");
    ss << "Score: " << score;
    text.setString(ss.str());
    text.setPosition(screen_width*0.05, screen_height*0.05);
    window.draw(text);

    // Time under minimap
    ss.str("");
    int minutes {};
    int seconds {};
    std::string minutes_string {"00"};
    std::string seconds_string {"00"};
    std::string milliseconds_string {"00"};
    if (time > (1000*60))
    {
        minutes = time / (1000*60);
        time -= (minutes*1000*60);
        if (minutes < 10)
        {
            minutes_string = "0" + std::to_string(minutes);
        }
        else
        {
            minutes_string = std::to_string(minutes);
        }
    }
    if (time > 1000)
    {
        seconds = time / 1000;
        time -= seconds * 1000; 
        if (seconds < 10)
        {
            seconds_string = "0" + std::to_string(seconds);
        }
        else
        {
            seconds_string = std::to_string(seconds);
        }
    }
    if (time < 10)
    {
        milliseconds_string = "00" + std::to_string(time);
    }
    else if (time < 100)
    {
        milliseconds_string = "0" + std::to_string(time);
    }
    else
    {
        milliseconds_string = std::to_string(time);
    }
    ss << "Time: " << minutes_string
       << ":"      << seconds_string
       << ":"      << milliseconds_string;
    text.setString(ss.str());
    text.setCharacterSize(screen_width/ 20);
    text.setPosition(0.7*screen_width, 0.3*screen_height);
    window.draw(text);
}

int Player::get_hp()
{
    return hp;
}

std::string Player::get_type_name() 
{
    return "player";
}
