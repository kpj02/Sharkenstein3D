#if !defined(POWER_UP)
#define POWER_UP

#include "Static.h"
#include "Data.h"
#include "Weapon.h"

class Power_Up : public Static
{
public:
    Power_Up(float const, float const, Data const&, Data_desc const&, float const&);
    ~Power_Up() = default;

    virtual void handle_collision(Game_Object*, Game_Object_Vector &) override;
    virtual std::shared_ptr<Weapon> get_weapon(std::shared_ptr<Game_Object> const&) = 0;
protected:
};

#endif