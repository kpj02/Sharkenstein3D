#ifndef STAR_H
#define STAR_H

#include "Static.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Star : public Static
{
public:
    Star(float const, float const, Data const&, Data_desc const&, float const& = 0);
    ~Star() = default;

    void handle_collision(Game_Object*, Game_Object_Vector &) override;
    std::string get_type_name() override;
    void update(sf::Time const&, Game_Object_Vector &) override;
    sf::Texture const* get_texture(float) override;
private:

};

#endif