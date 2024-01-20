#include "State.h"
#include <SFML/Audio.hpp>
#include <string>


State::State(std::string const filename)
: music_file{filename}
{}

void State::play_music(sf::Music & music)
{
    if (!music.openFromFile("resources/sounds/" + music_file))
    {
        throw std::logic_error("Failed to load music for state");
    }
    music.setVolume(40.f);
    music.play();
    music.setLoop(true);
}