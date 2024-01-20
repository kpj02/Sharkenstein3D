#if !defined(NHAJSCORE)
#define NHAJSCORE

#include <SFML/Graphics.hpp>
#include "State.h"
#include <string>
#include <vector>
#include <tuple>

/*
 * The state you enter when you win the game
 * The player enter their name, and their score + name is saved to a file 
 */

class New_hajscore : public State
{
public:
    New_hajscore(int const, int const);
    ~New_hajscore() = default;

    void handle(sf::Event const&, sf::RenderWindow&) override;
    void update(sf::RenderWindow&, sf::Time const&) override;
    void render(sf::RenderWindow&) override;
    void get_next_state(std::stack<std::shared_ptr<State>>&) override;
    void load_text();
    void load_data();
    void save_data();
private:
    std::string input_text;
    sf::Font font;
    sf::Text score_text;
    int incoming_score;
    std::string new_name;
    int new_time;
    std::vector<std::tuple<std::string, int, int>> hajscores;
};

#endif // NHAJSCORE