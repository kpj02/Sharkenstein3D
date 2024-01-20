#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include "Projectile.h"
#include  "Bullet.h"
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include "Chainsaw.h"
#include "Data.h"
#include <memory>
#include <limits>
#include <math.h>

/*
* Class that represents the enemies in the game
*/

Enemy::Enemy(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta,
             std::shared_ptr<Player> const& p_ptr, Map& map)
: Mortal{x, y, game_data, description, theta}, player_ptr{p_ptr}, map{map},
         weapon_cooldown{instance_stats.at("weapon_time")}, player_last_seen_at{position_vec}
{}
//Function that handle collition with other game objects
void Enemy::handle_collision(Game_Object* game_object, Game_Object_Vector&)
{
    //All enemy looses a live when hit with a missile/bullet or the chainsaw
    if(dynamic_cast<Projectile*>(game_object) || dynamic_cast<Chainsaw*>(game_object) || dynamic_cast<Bullet*>(game_object))
    {
        if (hp != 1)
        {
            hp -= 1;
        }
        else
        {
            expired = true;
        }
    }
}

//Setting the angle that the Enemy faces
void Enemy::set_angle(sf::Vector2f const& vec)
{
    if (vec.x > 0)
    {
        angle = 180 * std::atan(vec.y/ vec.x) / M_PI;
    }
    else
    {
        angle = 180 * std::atan(vec.y/ vec.x) / M_PI + 180;
    }
}

//Trigger all Enemy to a line of sight searching function.
void Enemy::search(sf::Time const& t_delta, Game_Object_Vector & object_vector)
{
    
    std::shared_ptr<Player> player = player_ptr.lock();

    // Vector that  points from enemy to player
    sf::Vector2f direction_vec {player->get_position() - position_vec};
    /*We incounter some problem like sharks shifiting direction very very 
    *cuickly when the shark is inside the player.
    */
    if (len(direction_vec) < std::numeric_limits<float>::epsilon()) 
    {
        return;
    }
    float enemy_player_len = len(direction_vec);

    //Normalize the vector from sharks to player
    direction_vec = direction_vec / len(direction_vec);

    /*
    * Vector thats orthogonal to the direction vector from enemy to player, 
    *gives the side shift for the rays layter 
    */
    float angle = std::atan2(direction_vec.y,direction_vec.x);
    sf::Vector2f plane_vec {static_cast<float>(-std::sin(angle)),static_cast<float>(std::cos(angle))};
    std::vector<sf::Vector2f> ray_dir {};
    std::vector<float> distance_vec {};
    std::vector<float> distance_wall {};

    /*
    *Loping over three rays, one directly from the shark to enemy, 
    *one to the right side and one to the left side from the direction that the shark is looking.
    *This "two line of sight" is importatt for the sharks to not go through walls. 
    *The shark is triggered when only both the left and right ray hits player 
    *without there bening a wall between any of the two rays.
    *If the player is seen a boolean is used for controlling diffrent enemys movement patterns.
    */
    for(int x {-1}; x<=1; x+=1)
    {
        float X = static_cast<float>(x);
        float distance_vec_loop = len(direction_vec*enemy_player_len);

        sf::Vector2f ray_dir_loop = {(direction_vec*enemy_player_len)/distance_vec_loop};

        ray_dir.push_back(ray_dir_loop);
        distance_vec.push_back(distance_vec_loop);

        //Calculats the square that the Enemy is standing in currently on the map
        int shark_x { static_cast<int>(std::floor((position_vec.x + radius*plane_vec.x*X) / (64*4)))};
        int shark_y { static_cast<int>(std::floor((position_vec.y + radius*plane_vec.y*X) / (64*4)))};

        int map_x {shark_x};
        int map_y {shark_y};

        float delta_x {std::abs(1/ray_dir_loop.x)};
        float delta_y {std::abs(1/ray_dir_loop.y)};

        int step_x {};
        int step_y {};

        float side_dist_x {};
        float side_dist_y {};

        if (ray_dir_loop.x < 0)
        {
            step_x = -1;
            side_dist_x = ((position_vec.x + radius*plane_vec.x*X)/256 - map_x) * delta_x;
        }
        else
        {
            step_x = 1;
            side_dist_x = (-(position_vec.x + radius*plane_vec.x*X)/256 + map_x + 1.0) * delta_x;

        }
        if (ray_dir_loop.y < 0)
        {
            step_y = -1;
            side_dist_y = ((position_vec.y + radius*plane_vec.y*X)/256 - map_y) * delta_y;
        }
        else
        {
            step_y = 1;
            side_dist_y = (-(position_vec.y + radius*plane_vec.y*X)/256 + map_y + 1.0) * delta_y;
        }

        int side {};

        while(true)
        {
            if(side_dist_x < side_dist_y)
            {
                side_dist_x += delta_x;
                map_x += step_x;
                side = 0;
            }
            else
            {
                side_dist_y += delta_y;
                map_y += step_y;
                side = 1;
            }
            if (map.map_tiles.at(map_x + map.col_nr * map_y)->description != ' ')
            {
                break;
            }   
        }

        if (side == 0)
        {
            distance_wall.push_back((side_dist_x-delta_x)*256);       
        }  
        else
        {
            distance_wall.push_back((side_dist_y-delta_y)*256);
        }
    }

    bool player_seen {distance_wall.at(0) > distance_vec.at(0) && distance_wall.at(2) > distance_vec.at(2)};
    
    if (player_seen) //The player is seen
    {
        player_last_seen_at = position_vec+direction_vec*enemy_player_len;
        if (enemy_player_len > radius + 0.8f * player->get_radius())
        {
            get_next_movement(t_delta, object_vector, direction_vec*enemy_player_len, distance_vec.at(1), player_seen);
        }
    }
    else  //The player is not seen
    {
        sf::Vector2f move_to {player_last_seen_at - position_vec};
        /*
        *Check if player has been seen before, knowing player is not currently visable
        *Takes a small step to the position player is last seen at
        */
        if (len(move_to) > t_delta.asSeconds() * speed)
        {
            get_next_movement(t_delta, object_vector, move_to, len(move_to), player_seen); 
        }
    }
    return;
}

//Returns the lenght of a vector
float Enemy::len(sf::Vector2f const vect)
{
    return std::sqrt(vect.x*vect.x + vect.y * vect.y);
}

//movement function for all enemys, the enemy takes a small delta step in a given direction
void Enemy::get_next_movement(sf::Time const& t_delta, Game_Object_Vector&,
                              sf::Vector2f const distance_vec, float const distance_vec_lenght, bool)
{
    position_vec += speed * t_delta.asSeconds() * distance_vec / distance_vec_lenght;
}