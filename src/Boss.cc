#include "Boss.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Projectile.h"
#include "Player.h"
#include "Map.h"
#include <cmath>
#include <iostream>
#include <string>
#include "Data.h"
#include "Star.h"
#include "Chainsaw.h"
#include "Bullet.h"

Boss::Boss(Data const& game_data, Data_desc const& description,
           float const& theta, std::shared_ptr<Player> const& p_ptr, Map& map)
: Enemy{game_data.boss_spawn.x, game_data.boss_spawn.y, game_data, description, theta, p_ptr, map}
{}

void Boss::handle_collision(Game_Object* game_object, Game_Object_Vector & game_objects)
{
    if(dynamic_cast<Projectile*>(game_object) || dynamic_cast<Chainsaw*>(game_object) || dynamic_cast<Bullet*>(game_object))
    {
        if (hp != 1 && invincibility <= 0)
        {
            hp -= 1;
            invincibility = 0.5;
        }
        else if (hp == 1 && invincibility <= 0)
        {
            game_objects.push_back(std::make_shared<Star>(position_vec.x, position_vec.y, game_data, Data_desc {"grand_star", "grand_star"}, 0));
            expired = true;
        }
    }
}

void Boss::update(sf::Time const& t_delta, Game_Object_Vector & object_vector)
{
    weapon_cooldown -= t_delta.asSeconds();
    invincibility -= t_delta.asSeconds();
    search(t_delta, object_vector);
}

//Moves the shark in the right direction
void Boss::get_next_movement(sf::Time const& t_delta, Game_Object_Vector & object_vector,
                              sf::Vector2f const distance_vec, float const distance_vec_lenght, bool player_is_seen)
{
    if (!player_is_seen || (hp >  5 && distance_vec_lenght > 700 )
                        || (hp <= 5 && distance_vec_lenght > 1500))
    {
        position_vec += speed * t_delta.asSeconds() * distance_vec / distance_vec_lenght;
    }
    set_angle(distance_vec);
    use_weapon(object_vector);
}    

//Fires weapon in the right direction if its loaded
void Boss::use_weapon(Game_Object_Vector & object_vector)
{
    if (weapon_cooldown > 0)
    {
        return;
    }
    weapon_cooldown = instance_stats.at("weapon_time");

    sf::Vector2f offset {};

    if (hp > 5)
    {
        offset.x = 2 * radius * cos(angle * M_PI / 180);
        offset.y = 2 * radius * sin(angle * M_PI / 180);
        offset += position_vec;

        object_vector.push_back(std::make_shared<Projectile>(offset.x, offset.y, game_data, Data_desc {"projectile", "projectile"}, angle));
        return;
    }
    // hp <= 5
    for (int i {-1}; i <= 1; ++i)
    {
        offset.x = 2 * radius * cos((angle + i * 30) * M_PI / 180);
        offset.y = 2 * radius * sin((angle + i * 30) * M_PI / 180);
        offset += position_vec;

        object_vector.push_back(std::make_shared<Projectile>(offset.x, offset.y, game_data, Data_desc {"projectile", "projectile"}, angle + i * 15));
    }
    
}

std::string Boss::get_type_name() 
{
    return "boss";
}

void Boss::render(sf::RenderWindow & window, sf::Vector2f based_vect, float player_angle, std::vector<float> len_to_wall)
{   
    Game_Object::render(window, based_vect, player_angle, len_to_wall);

    float straight_len {static_cast<float>(1/std::sqrt(2)*(based_vect.x + based_vect.y))};
    float perpendicular_len {static_cast<float>(1/std::sqrt(2)*(-based_vect.x + based_vect.y))};
    
    float sprite_height {(1280 / 2) * instance_stats.at("sprite_height") / straight_len};
    float half_sprite_width {(1280 / 4) * instance_stats.at("sprite_width") / straight_len};
    float float_point {2 * instance_stats.at("float_point") / straight_len * 256};

    float line_height {static_cast<float>((1280 / 2) / straight_len * 256)};
    float middle_screen_y {static_cast<float>(0.5 * 720)};
    float line_bottom {middle_screen_y + line_height / 2 - float_point};

    float middle_sprite_x {640 + perpendicular_len/straight_len * 640};
    sf::RectangleShape rectangle1;

    rectangle1.setSize(sf::Vector2f(2*half_sprite_width, sprite_height*0.1f));
    rectangle1.setOrigin(sf::Vector2f(half_sprite_width, sprite_height*0.05f));
    rectangle1.setPosition(middle_sprite_x, line_bottom-sprite_height);
    rectangle1.setFillColor(sf::Color{0,0,0});

    sf::RectangleShape rectangle2;
    rectangle2.setSize(sf::Vector2f(2*half_sprite_width*(hp/10.f),sprite_height*0.1f));
    rectangle2.setOrigin(sf::Vector2f(half_sprite_width, sprite_height*0.05f));
    rectangle2.setPosition(middle_sprite_x, line_bottom-sprite_height);
    rectangle2.setFillColor(sf::Color{255,0,0});

    window.draw(rectangle1);
    window.draw(rectangle2);
}