#include <SFML/Graphics.hpp>

#include "State.h"
#include "Hajscore.h"
#include "Game.h"
#include <fstream>
#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>
#include <iomanip>

Hajscore::Hajscore()
: State {"offspring.ogg"}, hajscores{}, font{}, score_text{}
{
    Hajscore::load_data();
    Hajscore::load_text();
}

void Hajscore::handle(sf::Event const&, sf::RenderWindow&)
{}

void Hajscore::update(sf::RenderWindow&, sf::Time const&)
{}

void Hajscore::render(sf::RenderWindow& window)
{
    //For the text: Set string, set char. size, set position, 
    //draw, repeat. 
    sf::Vector2u size {1280,720};
    score_text.setString("Hajscore");
    score_text.setCharacterSize(size.y / 6);
    unsigned int width = size.x / 2 - score_text.getLocalBounds().width / 2;
    unsigned int height = score_text.getLocalBounds().height / 2;
    score_text.setPosition(width, height);
    height += score_text.getLocalBounds().height;
    window.draw(score_text);
 
    score_text.setCharacterSize(size.y / 12);
    for (unsigned int i = 0; i < hajscores.size(); i++)
    {
        std::string name_string = std::get<0>(hajscores[i]);
        std::stringstream ss;
        ss << name_string
           << std::setfill(' ')
           << std::setw(11 - name_string.size())
           << ":"
           << std::setw(5) 
           << std::get<1>(hajscores[i]);
        

        int time {std::get<2>(hajscores[i])}; 
        int minutes {};
        int seconds {};
        std::string minutes_string {"00"};
        std::string seconds_string {"00"};
        std::string milliseconds_string {"00"};
        if (time > (1000*60))
        {
            minutes = time / (1000*60);
            time -= (minutes*1000*60);
            if (minutes < 10)
            {
                minutes_string = "0" + std::to_string(minutes);
            }
            else
            {
                minutes_string = std::to_string(minutes);
            }
        }
        if (time > 1000)
        {
            seconds = time / 1000;
            time -= seconds * 1000; 
            if (seconds < 10)
            {
                seconds_string = "0" + std::to_string(seconds);
            }
            else
            {
                seconds_string = std::to_string(seconds);
            }
        }
        if (time < 10)
        {
            milliseconds_string = "00" + std::to_string(time);
        }
        else if (time < 100)
        {
            milliseconds_string = "0" + std::to_string(time);
        }
        else
        {
            milliseconds_string = std::to_string(time);
        }
        ss << ", " << minutes_string << ":" << seconds_string << ":" << milliseconds_string;
        score_text.setString(ss.str());


        height += score_text.getLocalBounds().height;
        width = size.x / 2 - score_text.getLocalBounds().width / 2;

        score_text.setPosition(width, height);
        window.draw(score_text);
        if (i==4)
        {
            break;
        }
    }
}

void Hajscore::get_next_state(std::stack<std::shared_ptr<State>>& states)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        states.pop();
    }
}

void Hajscore::load_data()
{
    //Load the data from the csv file "hajscore.csv".
    std::ifstream ip("resources/data/hajscore.csv");
    if(!ip.is_open()) 
    {
        std::ofstream HajscoreFile("resources/data/hajscore.csv");
        HajscoreFile.close();
        return;
    }
    std::string name {};
    std::string score {};
    std::string time {};
    //Only load the first 5 scores, if there is more than 5 scores.
    int i {0};
    while(ip.good())
    {
        i += 1;
        getline(ip, name, ',');
        if (name == "")
        {
            return;
        }
        getline(ip, score, ',');
        getline(ip, time, '\n');
        hajscores.push_back(std::make_tuple(name, stoi(score), stoi(time)));
        if (i == 5)
        {
            break;
        }
    }
}
void Hajscore::load_text()
{
    if (!font.loadFromFile("resources/fonts/unispace_rg.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
        return;
    } 
    score_text.setFont(font);
    score_text.setFillColor(sf::Color::White);
    score_text.setOutlineColor(sf::Color::Black);
    score_text.setOutlineThickness(2);
}
