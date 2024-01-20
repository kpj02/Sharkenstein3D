#if !defined(STATE)
#define STATE

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stack>
#include <string>
#include <vector>
#include <memory>

/*  
 * Virtual class that represents the different states of the game
 * children: Main_Menu, Game, Hajscore, New_highscore,Game_Over
 */

class State
{
public:
    State(std::string const);
    virtual ~State() = default;
    virtual void play_music(sf::Music &);
    virtual void handle(sf::Event const&, sf::RenderWindow&) = 0;
    virtual void update(sf::RenderWindow&, sf::Time const&) = 0;
    virtual void render(sf::RenderWindow&) = 0;
    virtual void get_next_state(std::stack<std::shared_ptr<State>>&) = 0;

protected:
    std::string music_file;
};


#endif // STATE
