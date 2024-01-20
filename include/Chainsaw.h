#if !defined(CHAINSAW)
#define CHAINSAW

#include "Weapon.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Data.h"

class Chainsaw : public Weapon
{
public:
    Chainsaw(std::shared_ptr<Game_Object> const&, Data const&, Data_desc const&);

    void update(sf::Time const&,
                     Game_Object_Vector&) override;

    virtual std::string get_type_name() override;
    //virtual void map_collision(Map& map, Game_Object_Vector &) override;
    std::string get_status() override;
    void animate();
private:
    sf::IntRect uvRect;
    int blink;
    sf::Sound chainsaw_sound;
};

#endif