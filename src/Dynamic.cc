#include "Dynamic.h"
#include <SFML/Graphics.hpp>
#include <string>

Dynamic::Dynamic(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Game_Object{x, y, game_data, description, theta},
  speed{instance_stats.at("speed")}
{}
