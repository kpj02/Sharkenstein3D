#if !defined(PROGRAM)
#define PROGRAM

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stack>
#include <memory>
#include <string>

#include "State.h"

/*  
 * Class that represents the 'game'
 */

class Program
{
public:
    // The program is initilazied with the size of the default window
    Program();

    ~Program() = default;

    // The function that contains the main game-loop
    void start();

private:

    bool prog_is_running;

    sf::RenderWindow window;

    // The stack that contains pointers to diffrent game states, which we switch between
    std::stack<std::shared_ptr<State>> states;

    sf::Clock clock;
    
    sf::Music music;
};

#endif // PROGRAM