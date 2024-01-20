#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Static.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Data.h"

class Explosion : public Static
{
public:
    Explosion(float const, float const, Data const&, Data_desc const&, float const&);
    ~Explosion() = default;

    void update(sf::Time const&, Game_Object_Vector &) override;
    virtual std::string get_type_name() override;
private:
    float time_alive;
    sf::Sound explosion_sound;
};

#endif