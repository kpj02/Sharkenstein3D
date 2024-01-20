#include "Power_Up.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Data.h"
#include <memory>

Power_Up::Power_Up(float const x, float const y, Data const& game_data, Data_desc const& description, float const& theta)
: Static{x, y, game_data, description, theta}
{}

void Power_Up::handle_collision(Game_Object* game_object, Game_Object_Vector&)
{
    if (dynamic_cast<Player*>(game_object))
    {
        expired = true;
    }
}
