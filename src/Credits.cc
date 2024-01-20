#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "Credits.h"
#include "Main_Menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>

using namespace std;


Credits::Credits()
: State         {"theballadof.ogg"},
  view          {sf::Vector2f(1280/2, 720/2), sf::Vector2f(1280, 720)},
  exit_to_menu  {sf::Vector2f{500, 120}, sf::Vector2f{640, 640}, "resources/images/return_menu.png"},
  font          {},
  description   {},
  cursor_texture{},
  cursor_sprite {}
{
    set_up_text();

    //Setting up the cursor
    cursor_texture.loadFromFile("resources/images/cursor.png");
    cursor_sprite.setTexture(cursor_texture);
    cursor_sprite.setScale(sf::Vector2f(0.5, 0.5));
}

void Credits::handle(sf::Event const& event, sf::RenderWindow& window)
{
    // Check if any of the buttons on the menu has been pressed.
    if (event.type == sf::Event::EventType::MouseButtonPressed)
    {
        // Where in the window did the player click
        sf::Vector2f click {window.mapPixelToCoords(sf::Mouse::getPosition(window))};

        // Check if the player clicked on any of the buttons
        exit_to_menu.handle_click(click);
    }
}

void Credits::update(sf::RenderWindow& window, sf::Time const&)
{
    //Updating the cursors position
    cursor_sprite.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

void Credits::render(sf::RenderWindow& window)
{   
    window.setView(view);
    
    // Set the positio of the text, and draw it
    window.draw(description);

    // draw the buttons
    exit_to_menu.render(window);

    //Cursor rendering
    window.draw(cursor_sprite);
}

void Credits::get_next_state(std::stack<std::shared_ptr<State>>& states)
{
    // If any of the buttons has been pressed add a new state, to the state stack
    if (exit_to_menu.is_pressed())
    {
        states.pop();
    }
}

// Function that helps to set up the description text
void Credits::set_up_text()
{
    std::ifstream file;
    std::string line {};
    std::ostringstream oss {};

    if (!font.loadFromFile("resources/fonts/SuperMario256.ttf")) 
    {
        std::cout << "Error loading font" << std::endl;
    }

    file.open("resources/data/Credits.txt");
    while ( getline(file, line, '\n') )
    { 
        oss << " " << line << "\n";
    }
    file.close();

    description.setString(oss.str());
    description.setFont(font);
    description.setFillColor(sf::Color::White);
    description.setOutlineColor(sf::Color::Red);
    description.setOutlineThickness(2);
    description.setPosition(0,0);
    description.setOrigin(0,0);
}
