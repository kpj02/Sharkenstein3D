#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <list>
#include "Game_Object.h"
#include "Star.h"
#include "Player.h"
#include "Projectile.h"
#include "Static.h"
#include "Dynamic.h"
#include "Mortal.h"
#include "Map.h"
#include "State.h"
#include "Game.h"
#include "Map.h"
#include "Enemy.h"
#include "Melee.h"
#include "Hajscore.h"
#include "New_hajscore.h"
#include "Power_Up.h"
#include "Chainpowerup.h"
#include "Range.h"
#include "Boss.h"
#include "Data.h"
#include "Game_Over.h"
#include "Minipowerup.h"
#include "Shotgunpowerup.h"
#include "Gib.h"

Game::Game()
: State{"atdoomsgate.ogg"}, game_data{}, map{game_data.textures}, player{}, game_objects{},
  view{sf::Vector2f(screen_width / 2, screen_height / 2), sf::Vector2f(screen_width, screen_height)}, 
  score{}, clock{}, fps_font{}, fps_text{}, frames_since_last{}, 
  fps_update_time{0.5}, minimap_border{}
{   
    // Load and initialize the texture, stats, and sound that are used 
    load_textures();
    load_stats();
    load_sounds();

    set_up_fps_text();
    set_up_minimap_border();

    //Create a player, set the position later based on the map file
    player = std::make_shared<Player>(0, 0, game_data, Data_desc {"player", "player"}, 0, map);
    game_objects.push_back(player);
    player->init_weapon(game_objects);

    // Spawn Game_Objects from the Map
    for (auto const& map_tile : map.map_tiles)
    {   
        // The position of the current object in the game world
        float current_x {((map_tile -> x) + 0.5f)*4*64};
        float current_y {((map_tile -> y) + 0.5f)*4*64};

        // Depending on what map_tile was read create a diffrent game object on the corresponding position
        switch (map_tile -> description)
        {
        case 'S':  // Star
            game_objects.push_back(
                std::make_shared<Star>(current_x, current_y, game_data, Data_desc {"star", "star"}, 0));
            ++game_data.stars;
            map_tile -> description = ' ';
            break;
        case 'M':  // Melee shark
            game_objects.push_back(
                std::make_shared<Melee>(current_x, current_y, game_data, Data_desc {"melee", "melee"}, 0, player, map));
            map_tile -> description = ' ';
            break;
        case 'R':  // Range shark
            game_objects.push_back(
                std::make_shared<Range>(current_x, current_y, game_data, Data_desc {"range", "range"}, 0, player, map));
            map_tile -> description = ' ';
            break;
        case 'B':  // Boss shark spawn position
            game_data.boss_spawn = sf::Vector2f(current_x, current_y);
            map_tile -> description = ' ';
            break;
        case 'P':  // Player
            player->set_position(current_x, current_y);
            map_tile -> description = ' ';
            break;
        case 'C': // Chainpowerup
            game_objects.push_back(
                std::make_shared<Chainpowerup>(current_x, current_y, game_data,  Data_desc {"chainpowerup", "chainpowerup"}, 0));
            map_tile -> description = ' ';
            break;
        case 'm': // Minipoweup
            game_objects.push_back(
                std::make_shared<Minipowerup>(current_x, current_y, game_data, Data_desc {"minipowerup", "minipowerup"}, 0));
            map_tile -> description = ' ';
            break;
        case 's': // Shotgunpowerup
            game_objects.push_back(
                std::make_shared<Shotgunpowerup>(current_x, current_y, game_data, Data_desc {"shotgunpowerup", "shotgunpowerup"}, 0));
            map_tile -> description = ' ';
            break;
        default:
            // Add door
            if (isdigit(map_tile->description))
            {
                int door_number {std::stoi(std::string(1, map_tile->description))}; 
                game_data.doors[door_number].push_back(sf::Vector2i(map_tile->x, map_tile->y));
            }   
            break;
        }
    }
    
    // Set the number of stars the player has to collect for the boss to spawn
    player->set_stars_left(game_data);

    // Generate a texture for the minimap 
    map.build_texture();
}

void Game::handle(sf::Event const&, sf::RenderWindow&)
{}

void Game::update(sf::RenderWindow&, sf::Time const& t_delta)
{
    fps_update_time -= t_delta.asSeconds();
    frames_since_last += 1.f;

    // FPS-counter
    if (fps_update_time <= 0.f)
    {
        std::stringstream fps_string {};
        fps_update_time = 0.5;
        fps_string << frames_since_last / fps_update_time;
        fps_text.setString(fps_string.str());
        frames_since_last = 0;
    }

    // All the new game objects that are created during the update are stored here
    // and later appended to the main list of game_objects
    Game_Object_Vector new_game_objects {};

    // Check for and handle collisions between all game objects
    for (unsigned i { 0 }; i < game_objects.size(); ++i)
    {
        for (unsigned j { i + 1 }; j < game_objects.size(); ++j)
        {
            // If any two game objects has collided
            if (game_objects.at(i)->has_collided(game_objects.at(j).get()))
            {
                // Let the two game object handle the collision, take the other game object as input 
                game_objects.at(i)->handle_collision(game_objects.at(j).get(), new_game_objects);
                game_objects.at(j)->handle_collision(game_objects.at(i).get(), new_game_objects);
            }
        }
    }
   
    // Update time related functiosn and create new objects
    for (auto game_object : game_objects)
    {
        game_object->update(t_delta, new_game_objects);
    }

    // Check and handle collision with walls
    for (auto game_object : game_objects)
    {
        game_object->map_collision(map, new_game_objects);
    }

    // Delete game objects that are expired, and update the player score
    for (unsigned i { 0 }; i < game_objects.size();)
    {
        // Dont delete game_object if not expired
        if (!game_objects.at(i)->is_expired())
        {
            ++i;
            continue;
        }

        // Handle delete
        if (dynamic_cast<Melee*>(game_objects.at(i).get()))
        {
            score += 100.0;
        }
        else if (dynamic_cast<Range*>(game_objects.at(i).get()))
        {
            score += 200.0;
        }
        else if (dynamic_cast<Boss*>(game_objects.at(i).get()))
        {
            score += 500.0;
        }
        else if (dynamic_cast<Star*>(game_objects.at(i).get()))
        {
            int collected_stars {game_data.stars - player->get_stars_left()};
            map.open_doors(collected_stars);
            score += 25.0;
        }

        // Put expired game_object last and remove it
        std::swap(game_objects.at(i), game_objects.back());
        game_objects.pop_back();
    }

    // Add new objects to objects
    game_objects.insert(game_objects.end(),new_game_objects.begin(),new_game_objects.end());

    return;
}

void Game::render(sf::RenderWindow& window)
{   
    // Render the walls, floor and ceiling
    map.render_map(window, player->get_position(), player->get_angle());
    
    // Only render the game objects that are in the players fov (90 degrees)
    // do some calculations that are the same for every instans 
    float cos_var {static_cast<float>(std::cos(M_PI_4 - player->get_angle() * M_PI / 180))};
    float sin_var {static_cast<float>(std::sin(M_PI_4 - player->get_angle() * M_PI / 180))};
    
    // All the game_objects that are in the fov-cone are added to this list
    std::list< std::pair< float, std::shared_ptr<Game_Object> > > game_object_list;

    for ( auto iterator : game_objects)
    {
        // Do some linear alegra to decide if the object is in the fov 
        sf::Vector2f current_pos{iterator->get_position() - player->get_position()};
        sf::Vector2f based_vect{current_pos.x * cos_var - current_pos.y * sin_var, 
                                current_pos.x * sin_var + current_pos.y * cos_var};

        based_vect.x += iterator->get_radius();
        based_vect.y += iterator->get_radius();

        if (based_vect.x > 0 && based_vect.y > 0)
        {
            game_object_list.push_back( make_pair(current_pos.x * current_pos.x
                                                  + current_pos.y * current_pos.y, iterator));
            
        }
    }

    // The list is sorted so the object furthes away is first, and the closest last
    game_object_list.sort();
    game_object_list.reverse();

    // Render the individual game objects from furthes away to closest
    for (auto iterator : game_object_list)
    {
        sf::Vector2f current_pos{iterator.second->get_position() - player->get_position()};
        sf::Vector2f based_vect{current_pos.x * cos_var - current_pos.y * sin_var, 
                                current_pos.x * sin_var + current_pos.y * cos_var};

        iterator.second->render(window, based_vect, player->get_angle(), map.len_to_wall);
    }
    
    window.draw(fps_text);

    // Render the mini-map
    //black_box.setSize(sf::Vector2f(1280 * 0.3, 720 * 0.3));
    //black_box.setPosition(sf::Vector2f(1280 * 0.7, 0));
    //black_box.setFillColor(sf::Color::Black);
    window.draw(minimap_border);
    
    view.setCenter(player->get_position());
    view.setViewport(sf::FloatRect(0.7f, 0.f, 0.3f, 0.3f));
    view.zoom(4.f);
    window.setView(view);
    
    map.render_map_2D(window);
    player->render_2d(window);
    
    view.zoom(0.25f);
    view.setViewport(sf::FloatRect(0.f,0.f,1.f,1.f));
    
    view.setCenter(screen_width / 2, screen_height / 2);

    window.setView(view);
    
    // Render the HUD
    player->render_hud(window, score, clock.getElapsedTime().asMilliseconds());

    return;
}


void Game::get_next_state(std::stack<std::shared_ptr<State>>& states)
{
    // If escape is pressed, return to the main menu
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        states.pop();
    }
    // When the player wins, go to the screen where new highscores are entered
    if (player->has_won())
    {
        sf::Time time = clock.getElapsedTime();
        score -= static_cast<int>(time.asSeconds()) + 100*player->get_hp();
        states.pop();
        states.push(std::make_shared<New_hajscore>(score, time.asMilliseconds()));
    }
    // When a player dies go to the game over screen
    if (player->is_dead())
    {
        states.pop();
        states.push(std::make_shared<Game_Over>());
    }
}

// Load all the textures used for diffrent game_objectsSa
void Game::load_textures()
{
    add_texture("player", "player.png");
    add_texture("melee", "melee.png");
    add_texture("range", "range.png");
    add_texture("boss", "bosshaj.png");

    add_texture("explosion", "explosion.png");
    add_texture("projectile", "missile_back.png");
    add_texture("missile_front", "missile_front.png");
    add_texture("missile_side_front_left", "missile_side_front_left.png");
    add_texture("missile_side_left", "missile_side_left.png");
    add_texture("missile_side_back_left", "missile_side_back_left.png");
    add_texture("missile_side_front_right", "missile_side_front_right.png");
    add_texture("missile_side_right", "missile_side_right.png");
    add_texture("missile_side_back_right", "missile_side_back_right.png");
    add_texture("missile_back", "missile_back.png");

    add_texture("grand_star", "grand_star.png");
    add_texture("star_front","star_front.png");
    add_texture("star_front_left","star_front_left.png");
    add_texture("star_left","star_left.png");
    add_texture("star_back_left","star_back_left.png");
    add_texture("star_back","star_back.png");
    add_texture("star_back_right","star_back_right.png");
    add_texture("star_right","star_right.png");
    add_texture("star_front_right","star_front_right.png");

    add_texture("gib1","gib1.png");
    add_texture("gib2","gib2.png");
    add_texture("gib3","gib3.png");
    add_texture("gib4","gib4.png");
    add_texture("gib5","gib5.png");
    add_texture("gib6","gib6.png");
    add_texture("gib7","gib7.png");
    add_texture("gib8","gib8.png");
    add_texture("gib9","gib9.png");

    add_texture("rocket_launcher", "rocket_launcher.png");
    add_texture("bullet", "bullet.png");
    add_texture("chainsaw", "chainsaw.png");
    add_texture("chainpowerup", "chainpowerup.png");
    add_texture("minigun", "minigun.png");
    add_texture("minipowerup", "minipowerup.png");
    add_texture("shotgun", "shotgun.png");
    add_texture("shotgunpowerup", "shotgunpowerup.png");

    add_texture("no_texture", "no_texture.png");
    add_texture("wood_floor","water_damaged_wood.png");
    add_texture("floor_with_star","floor_star.png");
    add_texture("door", "single_door.png");
    add_texture("minimap_door", "door_minimap.png");
    add_texture("ceiling", "blue_tile.png");
    add_texture("wall", "futuristic_wall.png");
    add_texture("minimap_border", "minimap_border.png");
}

// Add a texture key and a corresponding texture to game_data.textures, if not possible
// add the no_texture image to the key
void Game::add_texture(std::string texture_key, std::string texture_source)
{
    if( !game_data.textures[texture_key].loadFromFile("resources/images/" + texture_source) )
    {
        game_data.textures[texture_key].loadFromFile("resources/images/no_texture.png");
    }
}

// Read all the stats for game_objects for the stats.csv file
void Game::load_stats()
{
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

    input_file.close();
}

// Load all the game sounds to the game_data struct
void Game::load_sounds()
{
    if (!game_data.sounds["explosion"].loadFromFile("resources/sounds/explosion2.ogg"))
    {
        throw std::logic_error("Could not load exlposion.ogg");
    }
    if (!game_data.sounds["damage"].loadFromFile("resources/sounds/minecraft_damage.ogg"))
    {
        throw std::logic_error("Could not load minecraft_damage.ogg");
    }
    if (!game_data.sounds["chainsaw_start_sound"].loadFromFile("resources/sounds/chainsaw_start_sound.ogg"))
    {
        throw std::logic_error("Could not load chainsaw_start_sound.ogg");
    }
}

void Game::set_up_fps_text()
{
    if (!fps_font.loadFromFile("resources/fonts/SuperMario256.ttf")) 
    std::cout << "Error loading font" << std::endl;

    fps_text.setFont(fps_font);
    fps_text.setFillColor(sf::Color(0,0,0,100));
    fps_text.setOutlineColor(sf::Color(255,255,255,100));
    fps_text.setOutlineThickness(2);
    fps_text.setPosition(5,0);
    fps_text.setOrigin(0,0);
}

void Game::set_up_minimap_border()
{
    minimap_border.setTexture(game_data.textures.at("minimap_border"));
    minimap_border.setOrigin(game_data.textures.at("minimap_border").getSize().x, 0);
    minimap_border.setPosition(screen_width, 0);
}