#include <SFML/Graphics.hpp>
#include <string>

#include "Button.h"

using namespace std;

Button::Button(sf::Vector2f size, sf::Vector2f pos, string file_name)
    : texture {}, sprite { size }, pressed { false }
{
    texture.loadFromFile(file_name);
    sprite.setTexture(&texture);
    sprite.setPosition(pos);
    sprite.setOrigin(size.x / 2, size.y / 2);
}

void Button::render(sf::RenderWindow& target) const
{
    target.draw(sprite);
}

// Takes the point where a user clicked on the screen, set the status of the button
//      to pressed if the player clicked on the button
void Button::handle_click(sf::Vector2f const& click)
{
    if (sprite.getGlobalBounds().contains(click))
    {
        pressed = true;
    }
}

// If the button has been pressed return true and un-press it, else return false
bool Button::is_pressed()
{
    if (pressed)
    {
        pressed = false;
        return true;
    }
    return false;
}

void Button::set_is_pressed(bool is_pressed)
{
    pressed = is_pressed;
}