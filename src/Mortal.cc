#include "Mortal.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Data.h"
#include <memory>
/*
*Class that represents all the objetct taht have lives (hp), 
*Invincibility frame works like a timer where Player or any Enemy dont take damaga during a certain time duration.
*/

Mortal::Mortal(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Dynamic{x, y, game_data, description, theta},
  hp {static_cast<int>(instance_stats.at("hp"))},
  invincibility {0}
{}
