#if !defined(GAME_OVER)
#define GAME_OVER

#include "State.h"
#include "Button.h"

class Game_Over: public State
{
public:
    Game_Over();
    ~Game_Over() = default;

    void handle(sf::Event const&, sf::RenderWindow&) override;
    void update(sf::RenderWindow&, sf::Time const&) override;
    void render(sf::RenderWindow&) override;
    void get_next_state(std::stack<std::shared_ptr<State>>&) override;
private:
    Button you_died;
    Button restart_game;
    Button main_menu;

    sf::Texture cursor_texture;
    sf::Sprite cursor_sprite;
};

#endif // GAME_OVER
