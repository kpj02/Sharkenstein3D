#if !defined(SHOTGUN)
#define SHOTGUN

#include <vector>
#include <stack>
#include <memory>

#include "Weapon.h"
#include "Data.h"

class Shotgun : public Weapon
{
public:
    Shotgun(std::shared_ptr<Game_Object> const&, Data const&, Data_desc const&);
    ~Shotgun() = default;

    void update(sf::Time const&,
                Game_Object_Vector&) override;
    virtual std::string get_type_name() override;
    std::string get_status() override;

    void animate();
private:
    int ammo;
    sf::IntRect uvRect;
};

#endif