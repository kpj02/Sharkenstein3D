#if !defined(GAME)
#define GAME

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <sstream>

#include "Game_Object.h"
#include "Map.h"
#include "State.h"
#include "Player.h"
#include "Data.h"

/*  
 * The main game
 * Has ownership over all the game objects and the map, has meber functions
 * to initialize the game based on external data-files.  
 */

class Game : public State
{
public:
    Game();
    ~Game() = default;

    void handle(sf::Event const&, sf::RenderWindow&) override;
    void update(sf::RenderWindow&, sf::Time const&) override;
    void render(sf::RenderWindow&) override;
    void get_next_state(std::stack<std::shared_ptr<State>>&) override;


private:
    // Struct that contains all the game data
    Data game_data;

    // Class that represents the static game map
    Map map;

    // Special pointer to the Player class
    std::shared_ptr<Player> player;

    // Vector containing all gameobjects, ie everything that isn't the map
    Game_Object_Vector game_objects;

    sf::View view;
    
    // The players score
    int score;
    sf::Clock clock;

    // Helper functions that initializes and load the textures and
    // sounds used in the game
    void load_textures();
    void load_stats();
    void load_sounds();
    void add_texture(std::string, std::string);

    sf::Font fps_font;
    sf::Text fps_text;
    void set_up_fps_text();

    float frames_since_last;
    float fps_update_time;

    sf::Sprite minimap_border;
    void set_up_minimap_border();
};

#endif // GAME
