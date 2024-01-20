#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "Dynamic.h"
#include <vector>

class Bullet : public Dynamic
{
public:
    Bullet(float const, float const, Data const&, Data_desc const&, float const&);
    void update(sf::Time const&, std::vector<std::shared_ptr<Game_Object>> &) override;
    void handle_collision(Game_Object*, std::vector<std::shared_ptr<Game_Object>> &) override;
    void map_collision(Map& map, std::vector<std::shared_ptr<Game_Object>> & game_objects) override;
    virtual std::string get_type_name() override;
private:
    
};

#endif