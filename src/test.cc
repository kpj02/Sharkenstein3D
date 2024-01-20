#include "catch.hpp"
#include <fstream>
#include <sstream>
#include "Enemy.h"
#include "Map.h"
#include "Data.h"
#include "Melee.h"
#include "Star.h"
#include "Projectile.h"
#include <iostream>

TEST_CASE("Kollision med spelare och enemy")
{
    // Initialize a data struct and load textures and sound that are used in tests
    Data game_data {};
    
    game_data.textures["no_texture"].loadFromFile("resources/images/no_texture.png");
    game_data.textures["rocket_launcher"].loadFromFile("resources/images/no_texture.png");
    game_data.sounds["damage"].loadFromFile("resources/sounds/minecraft_damage.ogg");

    std::ifstream input_file {"resources/data/stats.csv"};
    std::string line {};
    std::vector<std::string> stat_names {};

    if (!input_file.is_open())
    {
        throw(std::runtime_error("Error opening file."));
    }

    // Read stat_names
    std::getline(input_file, line);
    std::istringstream stat_names_iss {line};
    std::string stat_name {};
    std::getline(stat_names_iss, stat_name, ',');  // throw 0,0 because it is not a name of a stat
    while (std::getline(stat_names_iss, stat_name, ','))
    {
        stat_names.push_back(stat_name);
    }

    // Read stats
    while (std::getline(input_file, line))
    {
        std::istringstream row {line};
        std::string cell {};
        std::string class_name {};
        float value {};
        
        std::getline(row, class_name, ',');  // Get class name in first cell
        
        for (auto& stat_name : stat_names)  // Get stats
        {
            std::getline(row, cell, ',');
            std::istringstream cell_iss {cell};
            cell_iss >> value;
            game_data.stats[class_name][stat_name] = value;
        }
    }

    Map map {game_data.textures};
    std::vector<std::shared_ptr<Game_Object>> game_objects {};
    
    
    // Make a star 
    std::shared_ptr<Star> star = std::make_shared<Star>(0, 0, game_data, Data_desc {"no_texture", "star"}, 0);
    game_objects.push_back(star);
    game_data.stars = 1;
    
    // Make a player and initialize
    std::shared_ptr<Player> player = std::make_shared<Player>(0, 0, game_data, Data_desc {"no_texture", "player"}, 0, map);
    game_objects.push_back(player);
    player->init_weapon(game_objects);
    player->set_stars_left(game_data);

    // Make a melee enemy
    std::shared_ptr<Melee> melee = std::make_shared<Melee>(0, 0, game_data, Data_desc {"no_texture", "melee"}, 0, player, map);
    game_objects.push_back(melee);

    // Make a projectile
    std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(0, 0, game_data, Data_desc {"no_texture", "projectile"}, 0);
    game_objects.push_back(projectile);


    /*
     * BEGIN TESTING 
     */
    
    SECTION("Position and movment")
    {
        // Check that the you can move the player
        CHECK(!(player->get_position() == sf::Vector2f{1000.f,1000.f}));
        player->set_position(1000.f,1000.f);
        CHECK(player->get_position() == sf::Vector2f{1000.f,1000.f});
        player->set_position(0.f,0.f);
    }

    SECTION("Check collsion with enemy, missile and star")
    {
        // Check that the hp of the player is correctly assigned
        CHECK(player->get_hp() == game_data.stats["player"]["hp"]);

        // Check that the player has collided with the shark
        CHECK(player->has_collided(melee.get()));

        // Check that the player has collided with the star
        CHECK(player->has_collided(star.get()));

        //Check that player has collided with projectile
        CHECK(player->has_collided(projectile.get()));
        
        
    }

    SECTION("Handle collision, with enemy, missile and star")
    {
        // Update the player and handle the collision with melee
        player->update(sf::seconds(game_data.stats.at("player").at("invincibility") + 1.0f), game_objects);
        player->handle_collision(melee.get(),game_objects);

        // Check that the player has lost one HP    
        CHECK(player->get_hp() == game_data.stats["player"]["hp"] - 1);
        // Handle the collision between the player and the star
        player->handle_collision(star.get(),game_objects);

        // Update the player and handle the collision with projectile
        player->update(sf::seconds(game_data.stats.at("player").at("invincibility") + 1.0f), game_objects);
        player->handle_collision(projectile.get(),game_objects);

        // Check that the player has lost one HP    
        CHECK(player->get_hp() == game_data.stats["player"]["hp"] - 2);

        // Handle the collision between the player and the star
        player->handle_collision(star.get(),game_objects);
        
        // Check that the number of stars left have decresed with one
        CHECK(player->get_stars_left() == game_data.stars - 1);

        // Collide with the star again, (if you pick up a star with 0 stars left you should win)
        player->handle_collision(star.get(),game_objects);

        // Check that player has won!
        CHECK(player->has_won());


        //Checks if player has died when hp == 0
        while (player->get_hp() > 0)
        {
            player->update(sf::seconds(game_data.stats.at("player").at("invincibility") + 1.0f), game_objects);
            player->handle_collision(projectile.get(),game_objects);
        }
        CHECK(player->is_dead());
    }
    



}