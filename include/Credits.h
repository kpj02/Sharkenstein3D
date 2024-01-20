#if !defined(CREDITS)
#define CREDITS

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "State.h"

/*  
 * The main menu of the game, from where you can start, look at highscore 
 * or learn how to play the game
 */

class Credits : public State
{
public:
    Credits();
    ~Credits() = default;

    void handle(sf::Event const&, sf::RenderWindow&) override;
    void update(sf::RenderWindow&, sf::Time const&) override;
    void render(sf::RenderWindow&) override;
    void get_next_state(std::stack<std::shared_ptr<State>>&) override;
private:
    sf::View view;
    // The button that changes the state to the main game
    Button exit_to_menu;

    // The font used for text diplayed in the main menu
    sf::Font font;

    // The sf::text object that explains the how the game is played.
    sf::Text description;

    // Function that initializes description
    void set_up_text();

    sf::Texture cursor_texture;
    sf::Sprite cursor_sprite;
    
};

#endif // CREDITS
