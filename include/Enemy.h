#if !defined(ENEMY)
#define ENEMY

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Mortal.h"
#include "Player.h"
#include "Map.h"
#include "Data.h"

/*  
 * Virtual class that represents the different Enemys of the game
 * children: Meele, Range, Boss
 */

class Enemy : public Mortal
{
public:
    Enemy(float const, float const, Data const&, Data_desc const&, float const&, std::shared_ptr<Player> const&, Map&);
    virtual ~Enemy() = default;

    virtual void handle_collision(Game_Object*, Game_Object_Vector &) override;
    virtual void search(sf::Time const&, Game_Object_Vector & object_vector);
    virtual void get_next_movement(sf::Time const&, Game_Object_Vector &, sf::Vector2f const, float const, bool); //VARFÖR = 0????
    virtual void set_angle(sf::Vector2f const&);
    virtual void use_weapon(Game_Object_Vector & object_vector) = 0;
protected:
    std::weak_ptr<Player> player_ptr;
    Map& map;
    float weapon_cooldown;
    sf::Vector2f player_last_seen_at;
    float len(sf::Vector2f const);
};

#endif