#if !defined(BUTTON)
#define BUTTON

#include <SFML/Graphics.hpp>

/*  
 * Class that represents clickable buttons used in states.
 */

class Button
{
public:
    // Create a button, input is; size, position, name of texture
    Button(sf::Vector2f, sf::Vector2f, std::string);

    void render(sf::RenderWindow&) const;

    // Takes the point where a user clicked on the screen, set the status of the button
    //      to pressed if the player clicked on the button
    void handle_click(sf::Vector2f const&);

    // Return true if the button has been pressed
    bool is_pressed();

    void set_is_pressed(bool);

private:
    sf::Texture texture;
    sf::RectangleShape sprite;
    bool pressed;
};

#endif // BUTTON
