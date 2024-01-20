#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

#include "Map.h"
#include "Data.h"

class Game_Object
{
public:
    Game_Object(float const, float const, Data const&, Data_desc const&, float const& = 0.f);
    virtual ~Game_Object() = default;

    virtual float get_angle();
    virtual sf::Vector2f get_position();
    virtual bool has_collided(Game_Object* const) const;
    virtual void handle_collision(Game_Object*, std::vector<std::shared_ptr<Game_Object>> &);
    virtual void update(sf::Time const&, std::vector<std::shared_ptr<Game_Object>> &) = 0;
    virtual void render(sf::RenderWindow &, sf::Vector2f, float, std::vector<float>);
    virtual sf::Texture const* get_texture(float);
    virtual float get_radius();
    virtual bool is_expired() const;
    virtual void map_collision(Map& map, std::vector<std::shared_ptr<Game_Object>> &);
    virtual void set_up_sprite(sf::Texture&, float = 1);
    virtual std::string get_type_name() = 0;
    std::string get_data_string();

protected:
    Data_desc const data_desc;
    Data const& game_data;
    std::map<std::string,float> const& instance_stats;
    sf::Sprite sprite;
    sf::RenderTexture sprite_render_texture;
    sf::Vector2f position_vec;
    float angle;
    float radius;
    float wobble;
    bool expired;
};

typedef std::vector<std::shared_ptr<Game_Object>> Game_Object_Vector;

#endif
