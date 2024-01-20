#if !defined(MAIN_MENU)
#define MAIN_MENU

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "State.h"

/*  
 * The main menu of the game, from where you can start, look at highscore 
 * or learn how to play the game
 */

class Main_Menu : public State
{
public:
    Main_Menu();
    ~Main_Menu() = default;

    void handle(sf::Event const&, sf::RenderWindow&) override;
    void update(sf::RenderWindow&, sf::Time const&) override;
    void render(sf::RenderWindow&) override;
    void get_next_state(std::stack<std::shared_ptr<State>>&) override;
    bool change_menu();
    void change_secret(bool);
private:
    sf::View view;
    // The button that changes the state to the main game
    Button game_1;
    Button game_2;

    // The button that changes the state to where you can se the previous highscores
    Button haj_score_1;
    Button haj_score_2;
    Button background_1;
    Button background_2;
    Button credits_1;
    Button credits_2;
    Button exit_game_1;
    Button exit_game_2;
    Button description_1;
    Button description_2;
    Button secret_1;
    Button secret_2;
    sf::Texture no_shark_texture;
    sf::Sprite no_shark_sprite;
    sf::Texture sus_texture;
    sf::Sprite sus_sprite;
    bool is_menu_1;
    bool is_secret_on_1;
    bool is_secret_on_2;
    sf::Texture cursor_texture;
    sf::Sprite cursor_sprite;
};

#endif // MAIN_MENU
