#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <memory>

#include "Map.h"
#include "Mortal.h"
#include "Weapon.h"
#include "Data.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"  // Remove warning from enable_shared_from_this

class Player : public Mortal, public std::enable_shared_from_this<Player>
{
public:
    Player(float const, float const, Data const&, Data_desc const&, float const, Map&);

    void update(sf::Time const&, Game_Object_Vector &) override;
    void handle_collision(Game_Object*, Game_Object_Vector &) override;
    void map_collision(Map& map, Game_Object_Vector & game_objects) override;
    void get_next_movement(sf::Time const&);
    void init_weapon(Game_Object_Vector& game_objects);
    void set_stars_left(Data const&);
    bool has_won() const;
    bool is_dead() const;
    void set_position(float const, float const);
    void render_hud(sf::RenderWindow&, int, int);
    void render_2d(sf::RenderWindow&);
    int get_hp();
    int get_stars_left();
    virtual std::string get_type_name() override;
protected:
    int stars_left;
    float angular_velocity;
    std::shared_ptr<Weapon> weapon;
    sf::Vector2f next_movement;
    bool won_game;
    Map& map;
    sf::Font font;
    sf::Sound damage_sound;
};

#pragma GCC diagnostic pop

#endif