#if !defined(ROCKET_LAUNCHER)
#define ROCKET_LAUNCHER

#include <vector>
#include <stack>
#include <memory>

#include "Weapon.h"
#include "Data.h"

class Rocket_Launcher : public Weapon
{
public:
    Rocket_Launcher(std::shared_ptr<Game_Object> const&, Data const&, Data_desc const&);
    ~Rocket_Launcher() = default;

    void update(sf::Time const&,
                Game_Object_Vector&) override;
    virtual std::string get_type_name() override;
    std::string get_status() override;
    void animate();

protected:

private:
    sf::IntRect uvRect;
    float switch_time;
};

#endif