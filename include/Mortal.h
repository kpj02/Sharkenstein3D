#ifndef MORTAL_H
#define MORTAL_H

#include "Dynamic.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Data.h"

/*  
 * Virtual class that represents the different living/"killable" objects of the game
 * children: Player, Enemy
 */

class Mortal : public Dynamic
{
public:
    Mortal(float const, float const, Data const&, Data_desc const&, float const&);
    virtual ~Mortal() = default;

protected:
   int hp;
   float invincibility; 
};

#endif

