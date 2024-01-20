#include <SFML/Graphics.hpp>
#include "Program.h"
#include "Main_Menu.h"
#include <string>
#include <iostream>
#include <SFML/Audio.hpp>
#include "Game.h"
#include <unordered_map>
#include <stack>
#include <memory>

Program::Program() :
    prog_is_running{true}, 
    window{sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Hajar ibland oss", sf::Style::Fullscreen},
    states{},
    clock{},
    music{}
{   
    // Decide where the window is placed on the screen
    sf::Vector2i origin {0, 0};
    window.setPosition(origin);

    // Creates the main menu for the game
    states.push(std::make_shared<Main_Menu>());

    // Plays the music for the corresponding state
    states.top()->play_music(music);
}

void Program::start()
{
    // Pointer to the state that is currently active
    std::shared_ptr<State> active_state = states.top();

    // Turning off the cursor for custom cursor in menus
    window.setMouseCursorVisible(false);

    while (prog_is_running)
    {
        // Handle the music
        if (states.top() != active_state)
        {
            music.stop();
            states.top()->play_music(music);
        }

        // Update active state
        active_state = states.top();

        // Close the progam when the "kryss-knapp" is pressed
        sf::Event event {};
        while ( window.pollEvent(event) )
        {
            if ( event.type == sf::Event::Closed )
            {
                prog_is_running = false;
            }
            active_state->handle(event, window);
        }

        // Update the active state
        active_state->update(window, clock.restart());

        // Display
        window.clear();
        active_state->render(window);
        window.display();
        
        // Check if the active state should be changed
        active_state->get_next_state(states);
        
        if (states.empty())
        {
            prog_is_running = false;
        }
    }
    window.close();
}
