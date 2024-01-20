#ifndef GIB_H
#define GIB_H

#include "Static.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Data.h"

class Gib : public Static
{
public:
    Gib(float const, float const, Data const&, Data_desc const&, float const&);
    ~Gib() = default;

    void update(sf::Time const&, Game_Object_Vector &) override;
    virtual std::string get_type_name() override;
    sf::Texture const* get_texture(float) override;
private:
    float time_alive;
};

#endif