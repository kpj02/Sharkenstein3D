#if !defined(MINIGUN)
#define MINIGUN

#include <vector>
#include <stack>
#include <memory>
#include <string>

#include "Weapon.h"
#include "Data.h"

class Minigun : public Weapon
{
public:
    Minigun(std::shared_ptr<Game_Object> const&, Data const&, Data_desc const&);
    ~Minigun() = default;

    void update(sf::Time const&,
                Game_Object_Vector&) override;
    virtual std::string get_type_name() override;
    std::string get_status() override;

    void animate();
private:
    int ammo;
    sf::IntRect uvRect;
    int blink;
};

#endif