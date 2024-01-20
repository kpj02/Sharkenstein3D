#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include "Hajscore.h"
#include "Main_Menu.h"
#include "Credits.h"
#include <iostream>

using namespace std;


Main_Menu::Main_Menu()
: State {"menu_music.ogg"},
  view              {sf::Vector2f(640, 360), sf::Vector2f(1280, 720)},
  game_1            {sf::Vector2f{220, 76}, sf::Vector2f{1016, 263}, "resources/images/start_1.png"},
  game_2            {sf::Vector2f{160, 40}, sf::Vector2f{652, 462}, "resources/images/invisible_texture.png"},
  haj_score_1       {sf::Vector2f{220, 76}, sf::Vector2f{1016, 350}, "resources/images/hajscore_1.png"},
  haj_score_2       {sf::Vector2f{160, 40}, sf::Vector2f{652, 512}, "resources/images/invisible_texture.png"},
  background_1      {sf::Vector2f{1280, 720}, sf::Vector2f{640, 360}, "resources/images/mainmenu_1.png"},
  background_2      {sf::Vector2f{1280, 720}, sf::Vector2f{640, 360}, "resources/images/mainmenu_2.png"},
  credits_1         {sf::Vector2f{220, 76}, sf::Vector2f{1016, 437}, "resources/images/credits_1.png"},
  credits_2         {sf::Vector2f{160, 40}, sf::Vector2f{652, 562}, "resources/images/invisible_texture.png"},
  exit_game_1       {sf::Vector2f{220, 76}, sf::Vector2f{1016, 524}, "resources/images/exit_1.png"},
  exit_game_2       {sf::Vector2f{78, 32}, sf::Vector2f{652, 683}, "resources/images/invisible_texture.png"},
  description_1     {sf::Vector2f{332, 200}, sf::Vector2f{240, 320}, "resources/images/description_1.png"},
  description_2     {sf::Vector2f{332, 200}, sf::Vector2f{1100, 320}, "resources/images/description_2.png"},
  secret_1          {sf::Vector2f{34, 36}, sf::Vector2f{448, 683}, "resources/images/invisible_texture.png"},
  secret_2          {sf::Vector2f{44, 38}, sf::Vector2f{652, 628}, "resources/images/invisible_texture.png"},
  no_shark_texture  {},
  no_shark_sprite   {},
  sus_texture       {},
  sus_sprite        {},
  is_menu_1         {true},
  is_secret_on_1    {false},
  is_secret_on_2    {false},
  cursor_texture    {},
  cursor_sprite     {}
{
    //Setting up secrets
    no_shark_texture.loadFromFile("resources/images/mainmenu_1_secret.png");
    no_shark_sprite.setTexture(no_shark_texture);
    no_shark_sprite.setPosition(sf::Vector2f(1280, 0));

    sus_texture.loadFromFile("resources/images/mainmenu_2_secret.png");
    sus_sprite.setTexture(sus_texture);
    sus_sprite.setPosition(sf::Vector2f(1280, 0));

    //Setting up cursor
    cursor_texture.loadFromFile("resources/images/cursor.png");
    cursor_sprite.setTexture(cursor_texture);
    cursor_sprite.setScale(sf::Vector2f(0.5, 0.5));
}

void Main_Menu::handle(sf::Event const& event, sf::RenderWindow& window)
{
    // Check if any of the buttons on the menu has been pressed.
    if (event.type == sf::Event::EventType::MouseButtonPressed)
    {   
        // Where in the window did the player click
        sf::Vector2f click {window.mapPixelToCoords(sf::Mouse::getPosition(window))};

        // Check if the player clicked on any of the buttons
        if (is_menu_1)
        {
            haj_score_1.handle_click(click);
            game_1.handle_click(click);
            exit_game_1.handle_click(click);
            background_1.handle_click(click);
            credits_1.handle_click(click);
            secret_1.handle_click(click);
        }
        else
        {
            haj_score_2.handle_click(click);
            game_2.handle_click(click);
            exit_game_2.handle_click(click);
            background_2.handle_click(click);
            credits_2.handle_click(click);
            secret_2.handle_click(click);
        }
    }
}

void Main_Menu::update(sf::RenderWindow& window, sf::Time const&)
{
    //Updating the cursors position
    cursor_sprite.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

void Main_Menu::render(sf::RenderWindow& window)
{   
    window.setView(view);

    // draw the buttons
    if (is_menu_1)
    {
        background_1.render(window);
        window.draw(no_shark_sprite);
        game_1.render(window);
        haj_score_1.render(window);
        credits_1.render(window);
        exit_game_1.render(window);
        description_1.render(window);
        secret_1.render(window);
    }
    else
    {
        background_2.render(window);
        window.draw(sus_sprite);
        game_2.render(window);
        haj_score_2.render(window);
        credits_2.render(window);
        exit_game_2.render(window);
        description_2.render(window);
        secret_2.render(window);
    }
    window.draw(cursor_sprite);
}

void Main_Menu::get_next_state(std::stack<std::shared_ptr<State>>& states)
{
    // If any of the buttons has been pressed add a new state, to the state stack
    if (haj_score_1.is_pressed() || haj_score_2.is_pressed())
    {
        states.push(std::make_shared<Hajscore>());
        background_1.set_is_pressed(false);
        background_2.set_is_pressed(false);
    }
    else if (credits_1.is_pressed() || credits_2.is_pressed())
    {
        states.push(std::make_shared<Credits>());
        background_1.set_is_pressed(false);
        background_2.set_is_pressed(false);
    }
    else if (game_1.is_pressed() || game_2.is_pressed())
    {
        states.push(std::make_shared<Game>());
        background_1.set_is_pressed(false);
        background_2.set_is_pressed(false);
    }
    else if (exit_game_1.is_pressed() || exit_game_2.is_pressed())
    {
        states.pop();
        background_1.set_is_pressed(false);
        background_2.set_is_pressed(false);
    }
    else if (secret_1.is_pressed() || secret_2.is_pressed())
    {
        change_secret(is_menu_1);
        background_1.set_is_pressed(false);
        background_2.set_is_pressed(false);
    }
    else if (background_1.is_pressed() || background_2.is_pressed())
    {
        is_menu_1 = change_menu();
    }
}

bool Main_Menu::change_menu()
{
    return !is_menu_1;
}

void Main_Menu::change_secret(bool is_menu_1)
{
    if (is_menu_1 && !is_secret_on_1)
    {
        no_shark_sprite.setPosition(sf::Vector2f(0, 0));
        is_secret_on_1 = true;
    }
    else if (!is_menu_1 && !is_secret_on_2)
    {
        sus_sprite.setPosition(sf::Vector2f(0, 0));
        is_secret_on_2 = true;
    }
    else if (is_menu_1)
    {
        no_shark_sprite.setPosition(sf::Vector2f(1280, 0));
        is_secret_on_1 = false;
    }
    else if (!is_menu_1)
    {
        sus_sprite.setPosition(sf::Vector2f(1280, 0));
        is_secret_on_2 = false;
    }
}