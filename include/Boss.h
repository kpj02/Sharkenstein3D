#if !defined(BOSS)
#define BOSS

#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Data.h"

class Boss : public Enemy
{
public:
    Boss(Data const&, Data_desc const&, float const&, std::shared_ptr<Player> const&, Map&);
    ~Boss() = default;
    void handle_collision(Game_Object*, Game_Object_Vector &) override;
    void update(sf::Time const&, Game_Object_Vector &) override;
    void get_next_movement(sf::Time const&, Game_Object_Vector &, sf::Vector2f const, float const,bool) override;
    void use_weapon(Game_Object_Vector & object_vector) override;
    virtual std::string get_type_name() override;
    virtual void render(sf::RenderWindow &, sf::Vector2f, float, std::vector<float>) override;
private:
};

#endif