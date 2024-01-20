#if !defined(HAJSCORE)
#define HAJSCORE

#include <SFML/Graphics.hpp>
#include "State.h"
#include <string>
#include <vector>
#include <tuple>

/*
 * The state that shows the previous highscores
 * This can be entered from the main menu or after the player has won the game 
 */

class Hajscore : public State
{
public:
    Hajscore();
    ~Hajscore() = default;

    void handle(sf::Event const&, sf::RenderWindow&) override;
    void update(sf::RenderWindow&, sf::Time const&) override;
    void render(sf::RenderWindow&) override;
    void get_next_state(std::stack<std::shared_ptr<State>>&) override;
    void load_data();
    void load_text();
private:
    std::vector<std::tuple<std::string, int, int>> hajscores;
    sf::Font font;
    sf::Text score_text;
};

#endif // HAJSCORE