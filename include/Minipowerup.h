#if !defined(MINIPOWERUP)
#define MINIPOWERUP

#include "Power_Up.h"
#include "Data.h"

class Minipowerup : public Power_Up
{
public:
    Minipowerup(float const, float const, Data const&, Data_desc const&, float const&);
    ~Minipowerup() = default;

    virtual std::string get_type_name() override;
    std::shared_ptr<Weapon> get_weapon(std::shared_ptr<Game_Object> const&) override;
protected:

};

#endif