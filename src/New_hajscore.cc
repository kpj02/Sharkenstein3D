#include <SFML/Graphics.hpp>

#include "State.h"
#include "New_hajscore.h"
#include "Hajscore.h"
#include "Game.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <tuple>
#include <vector>

New_hajscore::New_hajscore(int init_incomingscore, int init_new_time)
: State {"fuel.ogg"}, input_text{}, font{}, score_text{}, incoming_score {init_incomingscore}, 
        new_name {""}, new_time {init_new_time}, hajscores{}
{
    New_hajscore::load_text();
    New_hajscore::load_data();
}

void New_hajscore::handle(sf::Event const& event, sf::RenderWindow&)
{
    //Read the input text from keyboard. If the user presses
    //the backspace key, whithout having any text in the input
    //than do nothing, otherwise erase the last character.
    if (event.type == sf::Event::TextEntered && input_text.size() < 10)
    {
        if (event.text.unicode < 128)
        {
            input_text += static_cast<char>(event.text.unicode);
            new_name = input_text;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && input_text.size() > 0)
    {
        input_text.pop_back();
        new_name = input_text;
    }
}

void New_hajscore::update(sf::RenderWindow&, sf::Time const&)
{}

void New_hajscore::render(sf::RenderWindow& window)
{
    sf::Vector2u size{1280,720};
    std::stringstream ss;

    //Write "Your score"
    ss << "Your score: " << incoming_score;
    score_text.setString(ss.str());
    unsigned int width = size.x / 2 - score_text.getLocalBounds().width / 2;
    score_text.setCharacterSize(size.y / 12);
    unsigned int height = score_text.getLocalBounds().height / 2;
    score_text.setPosition(width, height);
    score_text.setFillColor(sf::Color::White);
    window.draw(score_text);

    //Write "Your time"
    ss.str("");
    int time = new_time;
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
    ss << "Your time: " << minutes_string << ":" << seconds_string << ":" << milliseconds_string;
    score_text.setString(ss.str());
    height += 1.5*score_text.getLocalBounds().height;
    width = size.x / 2 - score_text.getLocalBounds().width / 2;
    score_text.setPosition(width, height);
    window.draw(score_text);
    
    //Write "Enter you name"
    score_text.setString("Enter your name:");
    height += 1.5*score_text.getLocalBounds().height;
    width = size.x / 2 - score_text.getLocalBounds().width / 2;
    score_text.setPosition(width, height);
    window.draw(score_text);
    //Write the input text
    score_text.setString(input_text);
    height += 1.5*score_text.getLocalBounds().height;
    width = size.x / 2 - score_text.getLocalBounds().width / 2;
    score_text.setPosition(width, height);
    score_text.setFillColor(sf::Color::Red);
    window.draw(score_text);

    //Wrie Press Enter to save
    score_text.setString("Press Enter to save");
    height += 1.5*score_text.getLocalBounds().height;
    width = size.x / 2 - score_text.getLocalBounds().width / 2;
    score_text.setPosition(width, height);
    score_text.setFillColor(sf::Color::White);
    window.draw(score_text);
    
}

void New_hajscore::get_next_state(std::stack<std::shared_ptr<State>>& states)
{   
    //If the user press enter, save the score and change the state
    //to the hajscore state. If there is no text in the input,
    //the name sets to "Finn" instead.
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        return;
    }
    if (new_name == "")
    {
        new_name = "Finn";
    }

    save_data();
    states.pop();
    states.push(std::make_shared<Hajscore>());
}

void New_hajscore::load_text()
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

void New_hajscore::save_data()
{
    //Insertion sort algorithm
    std::vector<std::tuple<std::string, int, int>> temp_hajscores;
    if (hajscores.size() > 0)
    {
        for (unsigned long int i = 0;i < hajscores.size(); i++)
        {
            if (std::get<1>(hajscores[i]) > incoming_score)
            {
                temp_hajscores.push_back(hajscores[i]);
            }
            else
            {
                temp_hajscores.push_back(std::make_tuple(new_name, incoming_score, new_time));
                for (unsigned long int j {i}; j < hajscores.size(); j++)
                {
                    temp_hajscores.push_back(hajscores[j]);
                }
                break;
            }
        if (temp_hajscores.size() == hajscores.size())
        {
            temp_hajscores.push_back(std::make_tuple(new_name, incoming_score, new_time));
        }
        }
    }
    else if (hajscores.size() == 0)
    {
        temp_hajscores.push_back(std::make_tuple(new_name, incoming_score, new_time));
    }

    hajscores = temp_hajscores;
    //Save the hajscores to the csv file.
    std::ofstream HajscoreFile("resources/data/hajscore.csv");
    if (hajscores.size() > 0)
    {
        for (unsigned long int i = 0; i < hajscores.size(); i++)
        {
            HajscoreFile << std::get<0>(hajscores[i]) << ","
                         << std::get<1>(hajscores[i]) << ","
                         << std::get<2>(hajscores[i]) << '\n';
        }
    }
    else
    {
        return;
    }

    HajscoreFile.close();
}

void New_hajscore::load_data()
{
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
    
    while(ip.good())
    {
        getline(ip, name, ',');
        if (name == "")
        {
            return;
        }
        getline(ip, score, ',');
        getline(ip, time, '\n');
        hajscores.push_back(std::make_tuple(name, stoi(score), stoi(time)));
    }
}
