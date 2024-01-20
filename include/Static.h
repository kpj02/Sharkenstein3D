#ifndef STATIC_H
#define STATIC_H

#include "Game_Object.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Data.h"

class Static : public Game_Object
{
public:
    Static(float const, float const, Data const&, Data_desc const&, float const& = 0);
    virtual ~Static() = default;

    virtual void update(sf::Time const&, Game_Object_Vector &) override;
protected:
    
};

#endif