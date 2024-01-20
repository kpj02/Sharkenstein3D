#include "Game_Object.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Data.h"

#ifndef DYNAMIC_H
#define DYNAMIC_H

class Dynamic : public Game_Object
{
public:
    Dynamic(float const, float const, Data const&, Data_desc const&, float const& = 0);
    virtual ~Dynamic() = default;

protected:
    float speed;
};

#endif // Dynamic