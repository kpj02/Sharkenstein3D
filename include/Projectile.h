#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "Dynamic.h"
#include <vector>

class Projectile : public Dynamic
{
public:
    Projectile(float const, float const, Data const&, Data_desc const&, float const&);
    void update(sf::Time const&, Game_Object_Vector &) override;
    void handle_collision(Game_Object*, Game_Object_Vector &) override;
    void map_collision(Map& map, Game_Object_Vector & game_objects) override;
    virtual std::string get_type_name() override;
    sf::Texture const* get_texture(float) override;
private:
    
};

#endif