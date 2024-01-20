#if !defined(MELEE)
#define MELEE

#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include <string>
#include <vector>
#include "Data.h"

class Melee : public Enemy
{
public:
    Melee(float const, float const, Data const&, Data_desc const&, float const&, std::shared_ptr<Player> const&, Map&);
    ~Melee() = default;

    void update(sf::Time const&, Game_Object_Vector &) override;
    void get_next_movement(sf::Time const&, Game_Object_Vector &, sf::Vector2f const, float const, bool) override;
    void use_weapon(Game_Object_Vector & object_vector) override;
    virtual std::string get_type_name() override;
private:
};

#endif