#include "Static.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Data.h"
#include <memory>

Static::Static(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Game_Object{x, y, game_data, description, theta}
{}

void Static::update(sf::Time const&, Game_Object_Vector&)
{}
