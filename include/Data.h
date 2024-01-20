#if !defined(DATA)
#define DATA

#include <string>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>

// These are the screen dimensions we use, for bigger screens the window is scaled up
const int screen_width = 1280;
const int screen_height = 720;

struct Data
{
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, std::map<std::string, float>> stats;
    std::map<std::string, sf::SoundBuffer> sounds;
    sf::Vector2f boss_spawn;
    int stars;
    std::map<int, std::vector<sf::Vector2i>> doors;
};

struct Data_desc
{
    std::string texture_string;
    std::string data_string;
};

#endif // DATA
