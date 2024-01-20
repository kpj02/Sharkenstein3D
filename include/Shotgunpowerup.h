#if !defined(SHOTGUNPOWERUP)
#define SHOTGUNPOWERUP

#include "Power_Up.h"
#include "Data.h"

class Shotgunpowerup : public Power_Up
{
public:
    Shotgunpowerup(float const, float const, Data const&, Data_desc const&, float const&);
    ~Shotgunpowerup() = default;

    virtual std::string get_type_name() override;
    std::shared_ptr<Weapon> get_weapon(std::shared_ptr<Game_Object> const&) override;
protected:

};

#endif