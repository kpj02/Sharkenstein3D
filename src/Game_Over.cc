#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "Main_Menu.h"
#include "Game_Over.h"
#include "Button.h"

#include <iostream>

Game_Over::Game_Over()
: State {"everlong.ogg"},
  you_died      {sf::Vector2f{1000, 300}, sf::Vector2f{640, 150}, "resources/images/you_died.png"},
  restart_game  {sf::Vector2f{500, 120}, sf::Vector2f{640, 350}, "resources/images/restart_button.png"},
  main_menu     {sf::Vector2f{500, 120}, sf::Vector2f{640, 500}, "resources/images/main_menu_button.png"},
  cursor_texture{},
  cursor_sprite {}
{
    //Setting up cursor
    cursor_texture.loadFromFile("resources/images/cursor.png");
    cursor_sprite.setTexture(cursor_texture);
    cursor_sprite.setScale(sf::Vector2f(0.5, 0.5));
}

void Game_Over::handle(sf::Event const& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::EventType::MouseButtonPressed)
    {
        sf::Vector2f click {window.mapPixelToCoords(sf::Mouse::getPosition(window))};
        
        restart_game.handle_click(click);        
        main_menu.handle_click(click);
    }
}

void Game_Over::update(sf::RenderWindow& window, sf::Time const&)
{
    //Update cursors position
    cursor_sprite.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

void Game_Over::render(sf::RenderWindow& window)
{   
    you_died.render(window);
    restart_game.render(window);
    main_menu.render(window);
    window.draw(cursor_sprite);
    return;
}

void Game_Over::get_next_state(std::stack<std::shared_ptr<State>>& states)
{
    if (restart_game.is_pressed())
    {
        states.pop();
        states.push(std::make_shared<Game>());
    }
    else if (main_menu.is_pressed())
    {
        states.pop();
    }
}